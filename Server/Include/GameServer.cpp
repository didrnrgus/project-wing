#include "GameServer.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

void FClient::ResetLobbyState()
{
	// 클라 CMultiplayManager::ResetPlayerAfterInGame() → FPlayerInfo::Init() 과 같은 값으로 맞춘다.
	ready = false;
	character = 0;
	for (int& item : items)
		item = PLAYER_ITEM_TYPE_DEFAULT_INDEX;

	inGame = false;
	dead = false;
	movingUp = false;
	boost = false;
	hp = 0.0f;
	distance = 0.0f;
	height = 0.0f;
	stunTimer = 0.0f;
	protectTimer = 0.0f;
}

double CGameServer::Now() const
{
	using namespace std::chrono;
	return duration<double>(steady_clock::now().time_since_epoch()).count();
}

bool CGameServer::Init(unsigned short _port)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("[Server] WSAStartup failed\n");
		return false;
	}

	mListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mListenSock == INVALID_SOCKET)
	{
		printf("[Server] socket failed: %d\n", WSAGetLastError());
		return false;
	}

	BOOL exclusive = TRUE;
	setsockopt(mListenSock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char*)&exclusive, sizeof(exclusive));

	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(_port);

	if (bind(mListenSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("[Server] bind failed: %d (port %d already in use?)\n", WSAGetLastError(), _port);
		return false;
	}

	if (listen(mListenSock, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("[Server] listen failed: %d\n", WSAGetLastError());
		return false;
	}

	printf("[Server] Listening on port %d\n", _port);

	// 먼저 listen 해 두고 데이터를 받는다. 그동안 들어온 접속은 backlog 에서 기다린다.
	if (!mGameData.Load())
		printf("[Server] game data load incomplete. using defaults for missing values.\n");

	mRunning = true;
	return true;
}

void CGameServer::Shutdown()
{
	for (auto& c : mClients)
	{
		if (c->sock != INVALID_SOCKET)
			closesocket(c->sock);
	}
	mClients.clear();

	if (mListenSock != INVALID_SOCKET)
	{
		closesocket(mListenSock);
		mListenSock = INVALID_SOCKET;
	}
	WSACleanup();
	printf("[Server] shutdown\n");
}

void CGameServer::Run()
{
	double lastTick = Now();

	while (mRunning)
	{
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(mListenSock, &readSet);
		for (auto& c : mClients)
		{
			if (!c->closing)
				FD_SET(c->sock, &readSet);
		}

		double untilTick = TICK_INTERVAL_SEC - (Now() - lastTick);
		if (untilTick < 0.0)
			untilTick = 0.0;
		timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = (long)(untilTick * 1000000.0);

		int ready = select(0, &readSet, nullptr, nullptr, &timeout);
		if (ready == SOCKET_ERROR)
		{
			printf("[Server] select failed: %d\n", WSAGetLastError());
			break;
		}

		if (ready > 0)
		{
			if (FD_ISSET(mListenSock, &readSet))
				AcceptClient();

			// AcceptClient 가 vector 를 늘릴 수 있으므로 인덱스로 돈다.
			for (size_t i = 0; i < mClients.size(); ++i)
			{
				FClient& c = *mClients[i];
				if (!c.closing && FD_ISSET(c.sock, &readSet))
					ReceiveFrom(c);
			}
		}

		double now = Now();
		if (now - lastTick >= TICK_INTERVAL_SEC)
		{
			float dt = (float)(now - lastTick);
			if (dt > 0.5f)
				dt = 0.5f;	// 디버거로 멈췄다가 재개해도 한 번에 크게 튀지 않도록
			lastTick = now;
			Tick(dt);
			CheckTimeouts();
		}

		RemoveClosedClients();
		FlushAll();
	}
}

//////////////////////////////////////////////////////////////////////////
// 네트워크

void CGameServer::AcceptClient()
{
	sockaddr_in addr = {};
	int addrLen = sizeof(addr);
	SOCKET sock = accept(mListenSock, (sockaddr*)&addr, &addrLen);
	if (sock == INVALID_SOCKET)
		return;

	BOOL noDelay = TRUE;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&noDelay, sizeof(noDelay));
	DWORD sendTimeout = 3000;
	setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&sendTimeout, sizeof(sendTimeout));

	auto client = std::make_unique<FClient>();
	client->sock = sock;
	client->id = mNextId++;
	client->lastRecvTime = Now();

	char ip[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
	printf("[Server] accept id: %d (%s)\n", client->id, ip);

	// 닉네임 요청. 응답(WHO_ARE_YOU_RES)이 와야 방에 들어온다.
	SendInt(*client, 0, ServerMessage::MSG_WHO_ARE_YOU_REQ, client->id);
	mClients.push_back(std::move(client));
}

void CGameServer::ReceiveFrom(FClient& _client)
{
	char buffer[4096];
	int received = recv(_client.sock, buffer, sizeof(buffer), 0);
	if (received <= 0)
	{
		_client.closing = true;
		return;
	}

	// 거절된 클라: 하트비트 등을 읽어서 버린다. 안 읽고 닫으면 RST 가 나가 거절 메시지가 유실될 수 있다.
	if (_client.rejected)
		return;

	_client.lastRecvTime = Now();
	_client.recvBuf.insert(_client.recvBuf.end(), buffer, buffer + received);

	size_t offset = 0;
	while (!_client.closing && _client.recvBuf.size() - offset >= sizeof(MessageHeader))
	{
		MessageHeader header;
		memcpy(&header, _client.recvBuf.data() + offset, sizeof(header));

		if (header.bodyLen < 0 || header.bodyLen > MAX_BODY_LEN)
		{
			printf("[Server] invalid bodyLen %d from id: %d\n", header.bodyLen, _client.id);
			_client.closing = true;
			break;
		}

		size_t total = sizeof(MessageHeader) + header.bodyLen;
		if (_client.recvBuf.size() - offset < total)
			break;

		HandleMessage(_client, header, _client.recvBuf.data() + offset + sizeof(MessageHeader));
		offset += total;
	}

	_client.recvBuf.erase(_client.recvBuf.begin(), _client.recvBuf.begin() + offset);
}

bool CGameServer::Flush(FClient& _client)
{
	size_t sent = 0;
	while (sent < _client.sendBuf.size())
	{
		int n = send(_client.sock, _client.sendBuf.data() + sent, (int)(_client.sendBuf.size() - sent), 0);
		if (n == SOCKET_ERROR || n == 0)
		{
			_client.sendBuf.clear();
			return false;
		}
		sent += n;
	}
	_client.sendBuf.clear();
	return true;
}

void CGameServer::FlushAll()
{
	for (auto& c : mClients)
	{
		if (!c->sendBuf.empty() && !Flush(*c))
			c->closing = true;	// 다음 루프에서 정리
	}
}

void CGameServer::CheckTimeouts()
{
	double now = Now();
	for (auto& c : mClients)
	{
		if (c->closing)
			continue;

		if (c->rejected)
		{
			if (now - c->rejectTime > REJECT_LINGER_SEC)
				c->closing = true;
		}
		else if (now - c->lastRecvTime > HEARTBEAT_TIMEOUT_SEC)
		{
			printf("[Server] heartbeat timeout id: %d\n", c->id);
			c->closing = true;
		}
	}
}

void CGameServer::RemoveClosedClients()
{
	// 정리 중에 보내는 DISCONNECT 등으로 다른 클라가 closing 될 수 있으니 없을 때까지 반복
	while (true)
	{
		auto it = std::find_if(mClients.begin(), mClients.end(),
			[](const std::unique_ptr<FClient>& c) { return c->closing; });
		if (it == mClients.end())
			break;

		std::unique_ptr<FClient> client = std::move(*it);
		mClients.erase(it);

		// 거절 사유 같은 남은 메시지를 보내고 닫는다.
		if (!client->sendBuf.empty())
			Flush(*client);
		shutdown(client->sock, SD_BOTH);
		closesocket(client->sock);

		printf("[Server] disconnect id: %d (%s)\n", client->id, client->nickname.c_str());

		if (!client->joined)
			continue;

		BroadcastInt(client->id, ServerMessage::MSG_DISCONNECT, client->id);

		if (GetJoinedCount() == 0)
		{
			ResetRoom();
			continue;
		}

		if (client->id == mOwnerId)
		{
			mOwnerId = -1;
			for (auto& c : mClients)
			{
				if (c->joined && !c->closing)
				{
					mOwnerId = c->id;
					break;
				}
			}
			if (mOwnerId != -1)
			{
				FClient* owner = FindClient(mOwnerId);
				if (owner)
					owner->ready = false;
				printf("[Server] new owner id: %d\n", mOwnerId);
				BroadcastInt(mOwnerId, ServerMessage::MSG_NEW_OWNER, mOwnerId);
			}
		}

		if (mPhase != ERoomPhase::Lobby)
			CheckGameOver();
	}
}

void CGameServer::Send(FClient& _client, int _senderId, int _msgType, const void* _body, int _bodyLen)
{
	if (_client.closing)
		return;

	MessageHeader header{ _senderId, _msgType, _bodyLen };
	const char* h = (const char*)&header;
	_client.sendBuf.insert(_client.sendBuf.end(), h, h + sizeof(header));
	if (_bodyLen > 0)
		_client.sendBuf.insert(_client.sendBuf.end(), (const char*)_body, (const char*)_body + _bodyLen);
}

void CGameServer::Broadcast(int _senderId, int _msgType, const void* _body, int _bodyLen, const FClient* _exclude)
{
	for (auto& c : mClients)
	{
		if (c->joined && c.get() != _exclude)
			Send(*c, _senderId, _msgType, _body, _bodyLen);
	}
}

void CGameServer::SendInt(FClient& _client, int _senderId, int _msgType, int _value)
{
	Send(_client, _senderId, _msgType, &_value, sizeof(_value));
}

void CGameServer::BroadcastInt(int _senderId, int _msgType, int _value)
{
	Broadcast(_senderId, _msgType, &_value, sizeof(_value));
}

void CGameServer::BroadcastFloat(int _senderId, int _msgType, float _value)
{
	Broadcast(_senderId, _msgType, &_value, sizeof(_value));
}

//////////////////////////////////////////////////////////////////////////
// 메시지 처리

void CGameServer::HandleMessage(FClient& _client, const MessageHeader& _header, const char* _body)
{
	const int len = _header.bodyLen;

	if (_header.msgType == ClientMessage::MSG_HEARTBEAT)
	{
		Send(_client, 0, ServerMessage::MSG_HEARTBEAT_ACK);
		return;
	}

	if (!_client.joined)
	{
		if (_header.msgType == ClientMessage::MSG_WHO_ARE_YOU_RES)
			HandleWhoAreYouRes(_client, _body, len);
		return;
	}

	const bool isLobby = mPhase == ERoomPhase::Lobby;
	const bool isOwner = _client.id == mOwnerId;

	switch (_header.msgType)
	{
	case ClientMessage::MSG_START:
		if (isLobby && isOwner)
			HandleStart(_client);
		break;

	case ClientMessage::MSG_PICK_CHARACTER:
		if (isLobby && len >= (int)sizeof(int))
		{
			memcpy(&_client.character, _body, sizeof(int));
			BroadcastInt(_client.id, ServerMessage::MSG_PICK_CHARACTER, _client.character);
		}
		break;

	case ClientMessage::MSG_PICK_ITEM:
		if (isLobby && len >= (int)sizeof(int) * 2)
		{
			int slotItem[2];
			memcpy(slotItem, _body, sizeof(slotItem));
			if (slotItem[0] >= 0 && slotItem[0] < PLAYER_ITEM_COUNT_MAX)
			{
				_client.items[slotItem[0]] = slotItem[1];
				Broadcast(_client.id, ServerMessage::MSG_PICK_ITEM, slotItem, sizeof(slotItem));
			}
		}
		break;

	case ClientMessage::MSG_PICK_MAP:
		if (isLobby && isOwner && len >= (int)sizeof(int))
		{
			memcpy(&mMapId, _body, sizeof(int));
			printf("[Server] map: %d\n", mMapId);
			BroadcastInt(_client.id, ServerMessage::MSG_PICK_MAP, mMapId);
		}
		break;

	case ClientMessage::MSG_READY:
	case ClientMessage::MSG_UNREADY:
		if (isLobby && !isOwner)
		{
			_client.ready = _header.msgType == ClientMessage::MSG_READY;
			printf("[Server] id: %d readyFlag: %d\n", _client.id, _client.ready);
			Broadcast(_client.id, _client.ready ? ServerMessage::MSG_READY : ServerMessage::MSG_UNREADY);
		}
		break;

	case ClientMessage::MSG_CHANGE_NICKNAME:
	{
		std::string nickname(_body, strnlen(_body, len));
		if (nickname.size() > NICKNAME_SIZE - 1)
			nickname.resize(NICKNAME_SIZE - 1);
		_client.nickname = nickname;
		// 보낸 사람 포함 전원. 클라는 바디 전체를 닉네임으로 쓰므로 널 없이 보낸다.
		Broadcast(_client.id, ServerMessage::MSG_CHANGE_NICKNAME, nickname.data(), (int)nickname.size());
		break;
	}

	case ClientMessage::MSG_MOVE_UP:
	case ClientMessage::MSG_MOVE_DOWN:
		_client.movingUp = _header.msgType == ClientMessage::MSG_MOVE_UP;
		break;

	case ClientMessage::MSG_BOOST_ON:
	case ClientMessage::MSG_BOOST_OFF:
		_client.boost = _header.msgType == ClientMessage::MSG_BOOST_ON;
		break;

	case ClientMessage::MSG_TAKE_DAMAGE:
		if (len >= (int)sizeof(float))
		{
			float damage;
			memcpy(&damage, _body, sizeof(float));
			HandleTakeDamage(_client, damage);
		}
		break;

	default:
		break;
	}
}

void CGameServer::HandleWhoAreYouRes(FClient& _client, const char* _body, int _len)
{
	std::string nickname(_body, strnlen(_body, _len));
	if (nickname.size() > NICKNAME_SIZE - 1)
		nickname.resize(NICKNAME_SIZE - 1);
	_client.nickname = nickname;

	if (mPhase != ERoomPhase::Lobby)
	{
		Reject(_client, "Game is in progress.");
		return;
	}

	if (GetJoinedCount() >= PLAYER_COUNT_MAX)
	{
		Reject(_client, "Room is full.");
		return;
	}

	_client.joined = true;
	_client.ResetLobbyState();

	if (mOwnerId == -1)
		mOwnerId = _client.id;

	printf("[Server] join id: %d nickname: %s (players: %d, owner: %d)\n",
		_client.id, _client.nickname.c_str(), GetJoinedCount(), mOwnerId);

	// CONNECTED 가 먼저 가야 클라가 ROOM_FULL_INFO 에서 내가 방장인지 판단할 수 있다.
	SendInt(_client, 0, ServerMessage::MSG_CONNECTED, _client.id);

	// 클라는 ROOM_FULL_INFO 를 받을 때마다 플레이어를 추가만 하므로 접속 시 한 번만 보낸다.
	std::vector<char> roomInfo;
	WriteRoomFullInfo(roomInfo);
	Send(_client, 0, ServerMessage::MSG_ROOM_FULL_INFO, roomInfo.data(), (int)roomInfo.size());

	std::vector<char> join(sizeof(int) + _client.nickname.size());
	memcpy(join.data(), &_client.id, sizeof(int));
	memcpy(join.data() + sizeof(int), _client.nickname.data(), _client.nickname.size());
	Broadcast(_client.id, ServerMessage::MSG_JOIN, join.data(), (int)join.size(), &_client);
}

void CGameServer::Reject(FClient& _client, const std::string& _reason)
{
	printf("[Server] reject id: %d (%s)\n", _client.id, _reason.c_str());
	Send(_client, 0, ServerMessage::MSG_CONNECTED_REJECT, _reason.data(), (int)_reason.size());

	// 보내고 송신만 닫는다(FIN). 클라가 끊거나 REJECT_LINGER_SEC 가 지나면 정리된다.
	if (!Flush(_client))
	{
		_client.closing = true;
		return;
	}
	shutdown(_client.sock, SD_SEND);
	_client.rejected = true;
	_client.rejectTime = Now();
}

void CGameServer::WriteRoomFullInfo(std::vector<char>& _out) const
{
	// int ownerId, int mapId, int playerCount,
	// 플레이어마다 { int id, bool ready(1B), int character, int items[3], char nickname[64] } = 81바이트, 패딩 없음
	auto writeBytes = [&_out](const void* _data, size_t _size)
		{
			_out.insert(_out.end(), (const char*)_data, (const char*)_data + _size);
		};

	int playerCount = GetJoinedCount();
	writeBytes(&mOwnerId, sizeof(int));
	writeBytes(&mMapId, sizeof(int));
	writeBytes(&playerCount, sizeof(int));

	for (auto& c : mClients)
	{
		if (!c->joined || c->closing)
			continue;

		bool ready = c->ready;
		char nickname[NICKNAME_SIZE] = {};
		strncpy_s(nickname, c->nickname.c_str(), NICKNAME_SIZE - 1);

		writeBytes(&c->id, sizeof(int));
		writeBytes(&ready, 1);
		writeBytes(&c->character, sizeof(int));
		writeBytes(c->items, sizeof(int) * PLAYER_ITEM_COUNT_MAX);
		writeBytes(nickname, NICKNAME_SIZE);
	}
}

void CGameServer::HandleStart(FClient& _client)
{
	bool allReady = true;
	for (auto& c : mClients)
	{
		if (c->joined && !c->closing && c->id != mOwnerId && !c->ready)
		{
			allReady = false;
			break;
		}
	}

	if (!allReady)
	{
		printf("[Server] start rejected: not all ready\n");
		SendInt(_client, _client.id, ServerMessage::MSG_START_ACK, 0);
		return;
	}

	StartGame();
}

void CGameServer::HandleTakeDamage(FClient& _client, float _damage)
{
	if (mPhase != ERoomPhase::Playing || !_client.inGame || _client.dead)
		return;

	// 클라도 스턴/보호 중에는 보내지 않지만, 지연으로 겹쳐 온 것은 무시한다.
	if (_client.stunTimer > 0.0f || _client.protectTimer > 0.0f)
		return;

	if (_client.hp <= 0.0f)
	{
		printf("[Server] ClientMessage::MSG_TAKE_DAMAGE Dead######## id: %d\n", _client.id);
		KillPlayer(_client, "collision with 0 hp");
		return;
	}

	float applied = (std::max)(0.0f, _damage - _client.def);
	_client.hp = (std::max)(0.0f, _client.hp - applied);
	_client.stunTimer = _client.stunDuration;
	_client.protectTimer = 0.0f;

	printf("[Server] ClientMessage::MSG_TAKE_DAMAGE id: %d damage: %.1f hp: %.1f\n", _client.id, applied, _client.hp);

	BroadcastFloat(_client.id, ServerMessage::MSG_TAKEN_DAMAGE, _client.hp);
	Broadcast(_client.id, ServerMessage::MSG_TAKEN_STUN);

	if (_client.hp <= 0.0f)
		KillPlayer(_client, "collision");
}

//////////////////////////////////////////////////////////////////////////
// 게임

void CGameServer::InitPlayerStat(FClient& _client)
{
	// 클라 CPlayerInGameObject::Init() 과 같은 방식: 캐릭터 기본값 + 장착 아이템 add_value
	const FCharacterStat& base = mGameData.GetCharacter(_client.character);
	_client.hp = base.HP;
	_client.speed = base.Speed;
	_client.dex = base.Dex;
	_client.def = base.Def;
	_client.stunDuration = base.StunDuration;

	for (int itemIndex : _client.items)
	{
		if (itemIndex < 0)
			continue;
		const FItemStat* item = mGameData.GetItem(itemIndex);
		if (!item)
			continue;

		switch (item->StatType)
		{
		case EStatType::HP:		_client.hp += item->AddValue; break;
		case EStatType::Speed:	_client.speed += item->AddValue; break;
		case EStatType::Dex:	_client.dex += item->AddValue; break;
		case EStatType::Def:	_client.def += item->AddValue; break;
		default: break;
		}
	}

	_client.inGame = true;
	_client.dead = false;
	_client.movingUp = false;
	_client.boost = false;
	_client.distance = 0.0f;
	_client.height = 0.0f;
	_client.stunTimer = 0.0f;
	_client.protectTimer = 0.0f;

	printf("[Server] id: %d _statInfo.HP: %.1f _statInfo.Speed: %.1f _statInfo.Dex: %.1f _statInfo.Def: %.1f\n",
		_client.id, _client.hp, _client.speed, _client.dex, _client.def);
}

void CGameServer::StartGame()
{
	if (!mGameData.IsLoaded())
		mGameData.Load();	// 시작 때 못 받았으면 한 번 더 시도

	for (auto& c : mClients)
	{
		if (c->joined && !c->closing)
			InitPlayerStat(*c);
	}

	mPhase = ERoomPhase::Countdown;
	mCountdownTimer = COUNTDOWN_SEC;
	mObstacleTimer = 0.0f;

	printf("[Server] game start (map: %d, players: %d)\n", mMapId, GetJoinedCount());
	BroadcastInt(mOwnerId, ServerMessage::MSG_START_ACK, 1);
}

void CGameServer::Tick(float _deltaTime)
{
	if (mPhase == ERoomPhase::Countdown)
	{
		mCountdownTimer -= _deltaTime;
		if (mCountdownTimer <= 0.0f)
		{
			mPhase = ERoomPhase::Playing;
			mObstacleTimer = 0.0f;
			printf("[Server] countdown finished\n");
			Broadcast(0, ServerMessage::MSG_COUNTDOWN_FINISHED);
		}
		return;
	}

	if (mPhase != ERoomPhase::Playing)
		return;

	std::vector<int> deadIds;

	for (auto& c : mClients)
	{
		if (!c->inGame || c->dead || c->closing)
			continue;

		if (c->stunTimer > 0.0f)
		{
			// 스턴: 이동, 거리, HP 감소 모두 멈춤. 끝나면 보호 시작.
			c->stunTimer -= _deltaTime;
			if (c->stunTimer <= 0.0f)
			{
				c->stunTimer = 0.0f;
				c->protectTimer = PROTECTION_SEC;
			}
		}
		else
		{
			// 거리 = Speed × dt × 0.01 × 부스트 (100px = 1m)
			float boost = c->boost ? BOOST_MULTIPLY : 1.0f;
			c->distance += c->speed * _deltaTime * 0.01f * boost;

			// 높이: 누르고 있으면 Dex 만큼 상승, 아니면 하강. 화면 안(-360 ~ 360)으로 제한.
			c->height += (c->movingUp ? 1.0f : -1.0f) * c->dex * _deltaTime;
			c->height = (std::max)(-360.0f, (std::min)(360.0f, c->height));

			if (c->protectTimer > 0.0f)
			{
				c->protectTimer -= _deltaTime;	// 보호 중에는 HP 감소 없음
			}
			else
			{
				// 시간당 HP 감소 = dt × (1 − Def/100)
				float decay = _deltaTime * (std::max)(0.0f, 1.0f - c->def / 100.0f);
				c->hp = (std::max)(0.0f, c->hp - decay);
				if (c->hp <= 0.0f)
					deadIds.push_back(c->id);
			}
		}

		BroadcastFloat(c->id, ServerMessage::MSG_PLAYER_DISTANCE, c->distance);
		BroadcastFloat(c->id, ServerMessage::MSG_PLAYER_HEIGHT, c->height);
		BroadcastFloat(c->id, ServerMessage::MSG_TAKEN_DAMAGE, c->hp);
	}

	for (int id : deadIds)
	{
		FClient* c = FindClient(id);
		if (c && c->inGame && !c->dead)
		{
			printf("[Server] DamagedPerDistance Dead id: %d\n", id);
			KillPlayer(*c, "hp decay");
		}
	}

	if (mPhase != ERoomPhase::Playing)
		return;	// 위에서 게임이 끝났을 수 있다

	// 장애물: 맵 obstacle_interval_time 마다 같은 값을 전원에게
	float interval = (std::max)(0.2f, mGameData.GetMap(mMapId).ObstacleIntervalTime);
	mObstacleTimer += _deltaTime;
	if (mObstacleTimer >= interval)
	{
		mObstacleTimer -= interval;

		// 클라 싱글 모드와 같은 범위: 크기 100~149, 회전 0~359, 높이 -360~359
		float obstacle[3];
		obstacle[0] = (float)(mRng() % 50) + 100.0f;
		obstacle[1] = (float)(mRng() % 360);
		obstacle[2] = (float)(mRng() % 720) - 360.0f;
		Broadcast(0, ServerMessage::MSG_OBSTACLE, obstacle, sizeof(obstacle));
	}
}

void CGameServer::KillPlayer(FClient& _client, const char* _reason)
{
	_client.dead = true;
	_client.hp = 0.0f;
	_client.movingUp = false;
	_client.boost = false;

	printf("[Server] player dead id: %d (%s) distance: %.1f\n", _client.id, _reason, _client.distance);

	BroadcastFloat(_client.id, ServerMessage::MSG_PLAYER_DISTANCE, _client.distance);
	BroadcastFloat(_client.id, ServerMessage::MSG_TAKEN_DAMAGE, 0.0f);
	Broadcast(_client.id, ServerMessage::MSG_PLAYER_DEAD);

	CheckGameOver();
}

void CGameServer::CheckGameOver()
{
	if (mPhase == ERoomPhase::Lobby)
		return;

	for (auto& c : mClients)
	{
		if (c->joined && !c->closing && c->inGame && !c->dead)
			return;
	}

	EndGame("All players dead. Game over.");
}

void CGameServer::EndGame(const std::string& _reason)
{
	printf("[Server] %s\n", _reason.c_str());

	// 클라가 바디를 char* 로 출력하므로 널까지 보낸다.
	Broadcast(0, ServerMessage::MSG_GAME_OVER, _reason.c_str(), (int)_reason.size() + 1);

	mPhase = ERoomPhase::Lobby;
	mCountdownTimer = 0.0f;
	mObstacleTimer = 0.0f;

	for (auto& c : mClients)
	{
		if (c->joined)
			c->ResetLobbyState();
	}
}

void CGameServer::ResetRoom()
{
	printf("[Server] room is empty. reset.\n");
	mOwnerId = -1;
	mMapId = 0;
	mPhase = ERoomPhase::Lobby;
	mCountdownTimer = 0.0f;
	mObstacleTimer = 0.0f;
}

FClient* CGameServer::FindClient(int _id)
{
	for (auto& c : mClients)
	{
		if (c->id == _id)
			return c.get();
	}
	return nullptr;
}

int CGameServer::GetJoinedCount() const
{
	int count = 0;
	for (auto& c : mClients)
	{
		if (c->joined && !c->closing)
			++count;
	}
	return count;
}
