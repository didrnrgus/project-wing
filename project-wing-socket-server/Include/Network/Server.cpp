#include "Server.h"
#include "Network/Client.h"
#include "Etc/DataStorageManager.h"

CServer::CServer()
{
}

CServer::~CServer()
{
}

void CServer::InitTimer()
{
	QueryPerformanceFrequency(&mSecond);
	QueryPerformanceCounter(&mTime);
}

float CServer::UpdateTimer()
{
	LARGE_INTEGER Time;
	QueryPerformanceCounter(&Time);
	mDeltaTime = (Time.QuadPart - mTime.QuadPart) / (float)mSecond.QuadPart;
	mTime = Time;
	mFPSTime += mDeltaTime;
	++mFPSTick;

	if (mFPSTick == 60)
	{
		mFPS = mFPSTick / mFPSTime;
		mFPSTick = 0;
		mFPSTime = 0.f;
	}

	return mDeltaTime;
}

float CServer::GetDeltaTimeFromTimer()
{
	return mDeltaTime;
}

bool CServer::Init()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	std::thread(&CServer::InGameUpdateLoop, this).detach();

	mServer = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	bind(mServer, (sockaddr*)&addr, sizeof(addr));
	listen(mServer, SOMAXCONN);

	std::cout << "\n";
	std::cout << "[Server] Listening on port " << PORT << "...\n";
	return true;
}

void CServer::Run()
{
	while (true)
	{
		sockaddr_in clientAddr{};
		int size = sizeof(clientAddr);
		SOCKET clientSock = accept(mServer, (sockaddr*)&clientAddr, &size);
		std::lock_guard<std::recursive_mutex> lock(mMutex);

		if ((int)mClients.size() >= MAX_PLAYERS)
		{
			const char* msg = "Room is full.";
			SendMsg(clientSock, 0, (int)ServerMessage::MSG_CONNECTED_REJECT, msg, strlen(msg) + 1);
			closesocket(clientSock);
			continue;
		}

		CClient* client = new CClient;
		client->sock = clientSock;
		client->id = mNextId++;
		mClients.push_back(client);
		client->Init();

		std::cout << "[Server] gClients.push_back(c); " << client->id << "\n";

		// 누구냐고 물어봄.
		SendMsg(clientSock, client->id, (int)ServerMessage::MSG_WHO_ARE_YOU_REQ, &client->id, sizeof(int));

		client->thread = std::thread(&CServer::ClientThread, this, client);
		client->thread.detach();
	}
	closesocket(mServer);
	WSACleanup();
}

bool CServer::SendAll(SOCKET sock, const char* data, int len)
{
	int sent = 0;

	while (sent < len)
	{
		int result = send(sock, data + sent, len - sent, 0);

		if (result == SOCKET_ERROR)
			return false;

		sent += result;
	}
	return true;
}

bool CServer::SendMsg(SOCKET sock, int senderId, int msgType, const void* body, int bodyLen)
{
	MessageHeader header{ senderId, msgType, bodyLen };

	if (!SendAll(sock, (char*)&header, sizeof(header)))
		return false;

	if (body && bodyLen > 0)
		return SendAll(sock, (char*)body, bodyLen);

	return true;
}

void CServer::Broadcast(int senderId, int msgType, const void* data, int len)
{
	for (auto& client : mClients)
		SendMsg(client->sock, senderId, msgType, data, len);
}

bool CServer::RecvAll(SOCKET sock, char* buffer, int len)
{
	int recvd = 0;

	while (recvd < len)
	{
		int result = recv(sock, buffer + recvd, len - recvd, 0);

		if (result <= 0)
			return false;

		recvd += result;
	}
	return true;
}

bool CServer::ReceiveMsg(SOCKET sock, MessageHeader& header, std::vector<char>& body)
{
	if (!RecvAll(sock, (char*)&header, sizeof(header)))
		return false;

	body.resize(header.bodyLen);

	if (header.bodyLen > 0)
		return RecvAll(sock, body.data(), header.bodyLen);

	return true;
}

void CServer::CheckGameOver()
{
	int aliveCount = 0;

	for (auto& client : mClients)
	{
		if (client->isAlive)
			aliveCount++;
	}

	if (aliveCount == 0)
	{
		for (auto& client : mClients)
		{
			auto _statInfo = CDataStorageManager::GetInst()->GetCharacterState(client->characterId);
			client->InitStat(_statInfo);
			client->Init();
		}
		mMapId = 0;
		mState = WAITING;
		const char* msg = "All players dead. Game over.";
		Broadcast(0, (int)ServerMessage::MSG_GAME_OVER, msg, strlen(msg) + 1);
	}
}

void CServer::SendRoomFullInfo(CClient* client)
{
	int playerCount = (int)mClients.size();
	int totalSize = sizeof(int)  // roomOwner
		+ sizeof(int) // mapId
		+ sizeof(int) // playerCount
		+ playerCount // 인원수 곱
		* (sizeof(int) // id
			+ sizeof(bool)  // ready
			+ sizeof(int)  // character
			+ sizeof(int) * 3 // item*3
			+ sizeof(char) * 64); // nickname

	std::vector<char> buffer(totalSize);
	char* ptr = buffer.data();

	memcpy(ptr, &mRoomOwner, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &mMapId, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &playerCount, sizeof(int));
	ptr += sizeof(int);


	for (auto& client : mClients)
	{
		memcpy(ptr, &client->id, sizeof(int));
		ptr += sizeof(int);

		memcpy(ptr, &client->isReady, sizeof(bool));
		ptr += sizeof(bool);

		memcpy(ptr, &client->characterId, sizeof(int));
		ptr += sizeof(int);

		memcpy(ptr, client->itemSlots, sizeof(int) * 3);
		ptr += sizeof(int) * 3;

		memcpy(ptr, client->nickname, sizeof(char) * 64);
		ptr += sizeof(char) * 64;
	}

	SendMsg(client->sock, 0, (int)ServerMessage::MSG_ROOM_FULL_INFO, buffer.data(), totalSize);
}

void CServer::InGameUpdateLoop()
{
	InitTimer();

	const float targetDelta = 1.0f / 30.0f;
	float broadcastAccumulated = 0.0f;

	const float countDownTime = 5.0f;
	float curCountDownTime = 0.0f;

	bool isFinishCountDown = false;

	while (true)
	{
		Sleep(1);
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		float dt = UpdateTimer(); // 이번 프레임 시간

		if (mState != RUNNING)
		{
			isFinishCountDown = false;
			continue;
		}

		broadcastAccumulated += dt;

		// 카운트다운 처리
		if (!isFinishCountDown)
		{
			curCountDownTime += dt;

			//std::cout
			//	<< " curCountDownTime " << curCountDownTime
			//	<< "\n";

			if (curCountDownTime < countDownTime)
				continue;
			else
			{
				isFinishCountDown = true;
				curCountDownTime = 0.0f;
				Broadcast(0, (int)ServerMessage::MSG_COUNTDOWN_FINISHED, nullptr, 0);
			}
		}

		// 🧠 스탯 업데이트는 매 프레임 처리
		for (auto& client : mClients)
		{
			if (!client->isAlive) continue;

			if (client->GetIsProtection())
			{
				client->ReleaseProtection(dt);
			}

			if (client->GetIsStun())
			{
				client->ReleaseStun(dt);
			}

			//std::cout << "client_" << client->id
			//	<< " client->GetIsStun(): " << client->GetIsStun() << "\n";

			if (!client->GetIsStun())
			{
				float _height = client->GetDex() * dt * (client->isMovingUp ? 1.0f : -1.0f);
				client->height += _height;
				client->height = clamp(client->height, SCREEN_HEIGHT * -0.5f, SCREEN_HEIGHT * 0.5f);

				//std::cout << "client_" << client->id
				//	<< " client->isMovingUp: " << client->isMovingUp
				//	<< " client->GetDex(): " << client->GetDex()
				//	<< " dt: " << dt
				//	<< " _height: " << _height
				//	<< " client->height: " << client->height << "\n";

				// 거리 & HP 갱신
				float speed = client->GetSpeed();
				float boostMultiplyValue = client->GetBoostValue();
				float speedPerFrame = speed * dt * 0.01f * boostMultiplyValue;
				client->AddPlayDistance(speedPerFrame);
			}

			if (!client->GetIsStun() && !client->GetIsProtection())
			{
#ifdef _DEBUG
				client->DamagedPerDistance(dt * 10.0f);
#else
				c->DamagedPerDistance(dt);
#endif
				if (client->isAlive && client->GetCurHP() <= 0.0f)
				{
					std::cout << "DamagedPerDistance Dead id: " << client->id << "\n";
					client->isAlive = false;
					mDeadPlayers.insert(client->id);
					Broadcast(client->id, (int)ServerMessage::MSG_PLAYER_DEAD, nullptr, 0);
					CheckGameOver();
				}
			}
		}

		for (auto& client : mClients)
		{
			if (!client->isAlive)
				continue;

			int currentStep = static_cast<int>(client->GetPlayDistance() / 16.0f);
			//std::cout << "client_" << client->id
			//	<< " client->lastObstacleStep: " << client->lastObstacleStep
			//	<< " currentStep: " << currentStep << "\n";

			if (currentStep > client->lastObstacleStep)
			{
				client->lastObstacleStep = currentStep;

				Obstacle obs;

				if (mObstaclesByStep.count(currentStep))
				{
					obs = mObstaclesByStep[currentStep];
				}
				else
				{
					obs.scale = rand() % 50 + 100.0f;
					obs.rotation = rand() % 360;
					obs.height = (rand() % (int)SCREEN_HEIGHT) - (SCREEN_HEIGHT * 0.5f);
					mObstaclesByStep.emplace(std::make_pair(currentStep, obs));
				}

				//std::cout << "client_" << client->id
				//	<< " scale: " << obs.scale
				//	<< " rotation: " << obs.rotation
				//	<< " scheightale: " << obs.height << "\n";

				// 다보낼 필요 없음.
				SendMsg(client->sock, client->id, ServerMessage::MSG_OBSTACLE, &obs, sizeof(obs));
			}
		}

		// 60FPS 기준으로 메시지 브로드캐스트
		while (broadcastAccumulated >= targetDelta)
		{
			broadcastAccumulated -= targetDelta;

			for (auto& client : mClients)
			{
				//if (!client->isAlive) continue;

				// 죽은 캐릭이라도 계속 보내야 함.
				float _dist = client->GetPlayDistance();
				float _height = client->height;
				float _curHp = client->GetCurHP();

				//std::cout << "client_" << client->id
				//	<< " _dist: " << _dist
				//	<< " _height: " << _height
				//	<< " _curHp: " << _curHp << "\n";

				if (mState == RUNNING)
				{
					Broadcast(client->id, (int)ServerMessage::MSG_PLAYER_DISTANCE, &_dist, sizeof(float));
					Broadcast(client->id, (int)ServerMessage::MSG_PLAYER_HEIGHT, &_height, sizeof(float));
					Broadcast(client->id, (int)ServerMessage::MSG_TAKEN_DAMAGE, &_curHp, sizeof(float));
				}
			}
		}
	}
}

void CServer::ClientThread(CClient* client)
{
	while (true)
	{
		MessageHeader header;
		std::vector<char> body;

		if (!ReceiveMsg(client->sock, header, body))
			break;

		std::lock_guard<std::recursive_mutex> lock(mMutex);

		switch ((ClientMessage::Type)header.msgType)
		{
		case ClientMessage::MSG_HEARTBEAT:
			SendMsg(client->sock, client->id, (int)ServerMessage::MSG_HEARTBEAT_ACK, nullptr, 0);
			break;
		case ClientMessage::MSG_WHO_ARE_YOU_RES:
		{
			// 닉네임 넣어주고
			//client->nickname.assign(body.begin(), body.end());
			strcpy_s(client->nickname, body.data());

			// 방장 여부 결정.
			if (mRoomOwner == -1)
				mRoomOwner = client->id;

			// 연결 되었음을 통지 / 방 정보 전달.
			SendMsg(client->sock, client->id, (int)ServerMessage::MSG_CONNECTED, &client->id, sizeof(int));
			SendRoomFullInfo(client);

			// 다른 사람들한테 신규 클라 정보 전달.
			int packetSize = sizeof(int) + sizeof(client->nickname);
			std::vector<char> buffer(packetSize);
			char* ptr = buffer.data();

			memcpy(ptr, &(client->id), sizeof(int));
			ptr += sizeof(int);

			memcpy(ptr, client->nickname, sizeof(client->nickname));
			ptr += sizeof(client->nickname);

			std::cout << "client_" << client->id
				<< " nickname: " << buffer.data() + sizeof(int)
				<< "\n";
			for (auto& other : mClients)
			{
				// MSG_JOIN 에다가 ID / 닉네임 태워서 보내야 함.
				if (other->id != client->id)
					SendMsg(other->sock, client->id, (int)ServerMessage::MSG_JOIN, buffer.data(), packetSize);
			}
			break;
		}
		case ClientMessage::MSG_START:
			if (client->id == mRoomOwner)
			{
				bool allReady = std::all_of(mClients.begin(), mClients.end(),
					[this](CClient* c)
					{
						return (c->id == mRoomOwner) || c->isReady;
					});

				if (allReady)
				{
					mState = RUNNING;
					mObstaclesByStep.clear();
					mDeadPlayers.clear();
					for (auto& client : mClients)
					{
						client->isAlive = true;

						// 스탯 계산해서 Init 하기.
						// 테이블 읽어서 기본 스텟 초기화.
						auto _statInfo = CDataStorageManager::GetInst()->GetCharacterState(client->characterId);

						std::cout << "client_" << client->id
							<< " _statInfo.HP: " << _statInfo.HP
							<< " _statInfo.Speed: " << _statInfo.Speed
							<< " _statInfo.Dex: " << _statInfo.Dex
							<< " _statInfo.Def: " << _statInfo.Def
							<< "\n";

						client->InitStat(_statInfo);

						std::cout << "client_" << client->id
							<< " c->GetHP: " << client->GetCurHP()
							<< " c->GetSpeed: " << client->GetSpeed()
							<< " c->GetDex: " << client->GetDex()
							<< " c->GetDef: " << client->GetDef()
							<< "\n";

						// 착용한 아이템 스텟에 적용.
						auto _itemDatas = CDataStorageManager::GetInst()->GetItemInfoDatas();
						int _itemLength = sizeof(client->itemSlots) / sizeof(client->itemSlots[0]);
						for (int i = 0; i < _itemLength; i++)
						{
							int _itemIndexInSlot = client->itemSlots[i];
							if (_itemIndexInSlot >= 0)
							{
								// 어떤 스탯에 얼마를 적용할것인지.
								client->AddValueByStatIndex(
									static_cast<EStatInfo::Type>(_itemDatas[_itemIndexInSlot].StatType)
									, _itemDatas[_itemIndexInSlot].AddValue);
							}
						}
					}

				}

				// 시작에 대한 결과를 알려줘야 함.
				int readyFlag = static_cast<int>(allReady);
				Broadcast(client->id, (int)ServerMessage::MSG_START_ACK, &readyFlag, sizeof(int));
			}
			break;

		case ClientMessage::MSG_READY:
			client->isReady = true;
			Broadcast(client->id, (int)ServerMessage::MSG_READY, nullptr, 0);
			break;

		case ClientMessage::MSG_UNREADY:
			client->isReady = false;
			Broadcast(client->id, (int)ServerMessage::MSG_UNREADY, nullptr, 0);
			break;

		case ClientMessage::MSG_PICK_CHARACTER:
			if (header.bodyLen == sizeof(int))
			{
				memcpy(&client->characterId, body.data(), sizeof(int));
				Broadcast(client->id, (int)ServerMessage::MSG_PICK_CHARACTER, body.data(), sizeof(int));
			}
			break;

		case ClientMessage::MSG_PICK_ITEM:
			if (header.bodyLen == sizeof(int) * 2)
			{
				int slot, itemId;
				memcpy(&slot, body.data(), sizeof(int));
				memcpy(&itemId, body.data() + sizeof(int), sizeof(int));
				if (slot >= 0 && slot < 3) client->itemSlots[slot] = itemId;
				Broadcast(client->id, (int)ServerMessage::MSG_PICK_ITEM, body.data(), sizeof(int) * 2);
			}
			break;

		case ClientMessage::MSG_PICK_MAP:
			if (client->id == mRoomOwner && header.bodyLen == sizeof(int))
			{
				memcpy(&mMapId, body.data(), sizeof(int));
				CDataStorageManager::GetInst()->SetSelectedMapIndex(mMapId);
				Broadcast(0, (int)ServerMessage::MSG_PICK_MAP, &mMapId, sizeof(int));
			}
			break;

		case ClientMessage::MSG_CHANGE_NICKNAME:
			//std::cout << "ClientMessage::MSG_CHANGE_NICKNAME id: " << client->id << "\n";
			strcpy_s(client->nickname, body.data());
			//std::cout << "ClientMessage::MSG_CHANGE_NICKNAME nickname: " << client->nickname << "\n";
			Broadcast(client->id, (int)ServerMessage::MSG_CHANGE_NICKNAME, body.data(), header.bodyLen);
			break;

		case ClientMessage::MSG_MOVE_UP:
			client->isMovingUp = true;
			Broadcast(client->id, (int)ServerMessage::MSG_MOVE_UP, nullptr, 0);
			//std::cout << "ClientMessage::MSG_MOVE_UP id: " << client->id << "\n";
			break;

		case ClientMessage::MSG_MOVE_DOWN:
			client->isMovingUp = false;
			Broadcast(client->id, (int)ServerMessage::MSG_MOVE_DOWN, nullptr, 0);
			//std::cout << "ClientMessage::MSG_MOVE_DOWN id: " << client->id << "\n";
			break;

		case ClientMessage::MSG_TAKE_DAMAGE:
			if (header.bodyLen == sizeof(float))
			{
				std::cout << "ClientMessage::MSG_TAKE_DAMAGE id: " << client->id << "\n";
				// 맵 테이블에 의한 데이지.
				float _damage = CDataStorageManager::GetInst()->GetSelectedMapInfo().CollisionDamage;
				client->SetStun();
				Broadcast(client->id, (int)ServerMessage::MSG_TAKEN_STUN, nullptr, 0);
				client->Damaged(_damage);

				struct { int id; float hp; } packetHp{ client->id, client->GetCurHP() };
				Broadcast(client->id, (int)ServerMessage::MSG_TAKEN_DAMAGE, &packetHp, sizeof(packetHp));

				if (client->isAlive && client->GetCurHP() <= 0.0f)
				{
					std::cout << "ClientMessage::MSG_TAKE_DAMAGE Dead######## id: " << client->id << "\n";
					client->isAlive = false;
					mDeadPlayers.insert(client->id);
					Broadcast(client->id, (int)ServerMessage::MSG_PLAYER_DEAD, nullptr, 0);
					CheckGameOver();
				}
			}
			break;

		case ClientMessage::MSG_BOOST_ON:
			client->SetIsBoostMode(true);
			Broadcast(client->id, (int)ServerMessage::MSG_BOOST_ON, nullptr, 0);
			break;

		case ClientMessage::MSG_BOOST_OFF:
			client->SetIsBoostMode(false);
			Broadcast(client->id, (int)ServerMessage::MSG_BOOST_OFF, nullptr, 0);
			break;

		default:
			break;
		}
	}

	{
		std::lock_guard<std::recursive_mutex> lock(mMutex);
		auto it = std::find_if(mClients.begin(), mClients.end()
			, [client](CClient* client)
			{
				return client->id == client->id;
			});

		if (it != mClients.end())
		{
			bool wasOwner = (client->id == mRoomOwner);
			mClients.erase(it);
			Broadcast(client->id, (int)ServerMessage::MSG_DISCONNECT, &client->id, sizeof(int));

			if (mClients.empty())
				mRoomOwner = -1;
			else if (wasOwner)
			{
				mRoomOwner = mClients.front()->id;
				Broadcast(0, (int)ServerMessage::MSG_NEW_OWNER, &mRoomOwner, sizeof(int));
			}
		}
	}

	closesocket(client->sock);
	delete client;
}
