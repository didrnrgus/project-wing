#pragma once
#include "NetworkManager.h"
#include "Etc/MultiplayManager.h"

#define PORT 12345
DEFINITION_SINGLE(CNetworkManager);

CNetworkManager::CNetworkManager()
{
	mIsConnection = false;
	mIsConnectCompleted = false;
}

CNetworkManager::~CNetworkManager()
{
	Clear();
	WSACleanup();
}

bool CNetworkManager::ConnetServer()
{
	if (WSAStartup(MAKEWORD(2, 2), &mWsaData) != 0) return false;

	mSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSock == INVALID_SOCKET) return false;

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_port = htons(PORT);

	inet_pton(AF_INET, gIPAddress.c_str(), &mServerAddr.sin_addr);

	mIsConnection = true;
	if (connect(mSock, (sockaddr*)&mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR) return false;

	CLog::PrintLog("CNetworkManager::ConnetServer()" + gIPAddress);

	mRecvThread = std::make_unique<std::thread>(&CNetworkManager::ReceiveThread, this, mSock);
	mHbThread = std::make_unique<std::thread>(&CNetworkManager::HeartbeatThread, this, mSock);

	return true;
}

void CNetworkManager::SendMsg(int senderId, int msgType, const void* body, int bodyLen)
{
	MessageHeader header{ senderId, msgType, bodyLen };
	SendAll(mSock, (char*)&header, sizeof(header));
	if (body && bodyLen > 0) SendAll(mSock, (char*)body, bodyLen);
}

bool CNetworkManager::PollMessage(RecvMessage& out)
{
	std::lock_guard<std::mutex> lock(mQueueMutex);
	if (mMessageQueue.empty()) return false;
	out = std::move(mMessageQueue.front());
	ProcessMessage(out);
	mMessageQueue.pop();
	return true;
}

void CNetworkManager::Clear(std::function<void()>&& Func)
{
	Clear();

	if (Func != nullptr)
	{
		Func();
	}
}

void CNetworkManager::Clear()
{
	CLog::PrintLog("CNetworkManager::Clear()");

	mIsConnection = false;
	mIsConnectCompleted = false;
	SetIsMultiplay(false);

	if (mSock == INVALID_SOCKET)
		return;

	if (mRecvThread && mRecvThread->joinable())
		mRecvThread->join();
	if (mHbThread && mHbThread->joinable())
		mHbThread->join();

	closesocket(mSock);

}

void CNetworkManager::ProcessMessage(const RecvMessage& msg)
{
	// 프레임워크 제한받지않고 메시지 받음.
	// 플레이어 관련 메시지는 여기서 해도 될거같은데,, 
	// -> 어차피 UI트리거등은 해당 씬에서 이벤트 받아야함.
	// 
	// 로비 전용 메시지들은 로비에서 구현하자.
	// 인게임 메시지도 마찬가지로 인게임에서 구현하자.
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_CONNECTED:
	{
		mIsConnectCompleted = true;
		int id;
		if (msg.body.size() >= sizeof(int))
			memcpy(&id, msg.body.data(), sizeof(int));

		CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] MSG_CONNECTED MyID: " + std::to_string(id));

		//CMultiplayManager::GetInst()->AddPlayer(id);
		CMultiplayManager::GetInst()->SetMyId(id);
		break;
	}

	case (int)ServerMessage::MSG_ROOM_FULL_INFO: // 이거 받기전까진 타이틀에 머무른다.
	{
		if (msg.body.size() >= sizeof(int) * 3)
		{
			const char* ptr = msg.body.data();
			int ownerId, mapId, playerCount;
			memcpy(&ownerId, ptr, sizeof(int)); ptr += sizeof(int);
			memcpy(&mapId, ptr, sizeof(int)); ptr += sizeof(int);
			memcpy(&playerCount, ptr, sizeof(int)); ptr += sizeof(int);
			CLog::PrintLog("[ROOM_INFO] Owner: " + std::to_string(ownerId) 
									+ ", Map: " + std::to_string(mapId) 
									+ ", Players: " + std::to_string(playerCount));

			for (int i = 0; i < playerCount; ++i)
			{
				int id, items[3];
				bool ready;
				memcpy(&id, ptr, sizeof(int)); ptr += sizeof(int);
				memcpy(&ready, ptr, sizeof(bool)); ptr += sizeof(bool);
				memcpy(items, ptr, sizeof(int) * 3); ptr += sizeof(int) * 3;
				CLog::PrintLog(+ "  Player " + std::to_string(id) + 
					" - Ready: " + (ready ? "Yes" : "No") + 
					", Items: [" + std::to_string(items[0]) + ", " + std::to_string(items[1]) + ", " + std::to_string(items[2]) + "]");

				CMultiplayManager::GetInst()->AddPlayer(id);
				CMultiplayManager::GetInst()->SetPlayerIsReadyFromId(id, ready);
				CMultiplayManager::GetInst()->SetPlayerItemFromId(id, 0, items[0]);
				CMultiplayManager::GetInst()->SetPlayerItemFromId(id, 1, items[1]);
				CMultiplayManager::GetInst()->SetPlayerItemFromId(id, 2, items[2]);
			}

			CMultiplayManager::GetInst()->SetHostFromId(ownerId);
			CMultiplayManager::GetInst()->SetCurMapIndex(mapId);
		}
		break;
	}

	case (int)ServerMessage::MSG_DISCONNECT:
	{
		mIsConnection = false;
		mIsConnectCompleted = false;
		int id;
		if (msg.body.size() >= sizeof(int))
			memcpy(&id, msg.body.data(), sizeof(int));

		CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] MSG_DISCONNECT ID: " + std::to_string(id));

		CMultiplayManager::GetInst()->RemovePlayer(id);
		break;
	}

	case (int)ServerMessage::MSG_CONNECTED_REJECT:
	{
		std::string reason(msg.body.begin(), msg.body.end());
		CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] MSG_CONNECTED_REJECT Reason: " + reason);
		break;
	}

	case (int)ServerMessage::MSG_NEW_OWNER:
	{
		int id;
		if (msg.body.size() >= sizeof(int))
			memcpy(&id, msg.body.data(), sizeof(int));

		CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] MSG_NEW_OWNER: " + std::to_string(id));

		CMultiplayManager::GetInst()->SetHostFromId(id);
		break;
	}

	case (int)ServerMessage::MSG_JOIN:
	{
		// 방에 다른애가 들어옴.
		int newId;
		if (msg.body.size() >= sizeof(int))
			memcpy(&newId, msg.body.data(), sizeof(int));

		CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] New client MSG_JOIN: " + std::to_string(newId));

		CMultiplayManager::GetInst()->AddPlayer(newId);
		break;
	}

	/////////////////로비 플레이어 / 맵 세팅////////////////////////////////////////
	case (int)ServerMessage::MSG_PICK_MAP:
	{
		int mapId;
		if (msg.body.size() >= sizeof(int))
			memcpy(&mapId, msg.body.data(), sizeof(int));

		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] MSG_PICK_MAP " + std::to_string(mapId));

		CMultiplayManager::GetInst()->SetCurMapIndex(mapId);
		break;
	}

	case (int)ServerMessage::MSG_PICK_ITEM:
	{
		// 이건 msg.senderId 도 중요하다.
		int slot, itemId;
		if (msg.body.size() >= sizeof(int) * 2)
		{
			memcpy(&slot, msg.body.data(), sizeof(int));
			memcpy(&itemId, msg.body.data() + sizeof(int), sizeof(int));
		}
		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] Player " + std::to_string(msg.senderId) + " MSG_PICK_ITEM " + std::to_string(itemId) + " in slot " + std::to_string(slot));

		CMultiplayManager::GetInst()->SetPlayerItemFromId(msg.senderId, slot, itemId);
		break;
	}

	case (int)ServerMessage::MSG_PICK_CHARACTER:
	{
		int characterId;
		if (msg.body.size() >= sizeof(int))
			memcpy(&characterId, msg.body.data(), sizeof(int));

		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] Player " + std::to_string(msg.senderId) + " MSG_PICK_CHARACTER " + std::to_string(characterId));

		CMultiplayManager::GetInst()->SetPlayerCharacterFromId(msg.senderId, characterId);
		break;
	}

	//////////////////////로비 게임 시작관련///////////////////////////////////
	case (int)ServerMessage::MSG_READY:
		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] MSG_READY " + std::to_string(msg.senderId));

		CMultiplayManager::GetInst()->SetPlayerIsReadyFromId(msg.senderId, true);
		break;

	case (int)ServerMessage::MSG_UNREADY:
		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] MSG_UNREADY " + std::to_string(msg.senderId));

		CMultiplayManager::GetInst()->SetPlayerIsReadyFromId(msg.senderId, false);
		break;

	case (int)ServerMessage::MSG_START_ACK:
		CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_START_ACK");

		CMultiplayManager::GetInst()->SetIsGameStart(true);
		break;

		//////////////////////인게임 게임로직///////////////////////////////////
	case (int)ServerMessage::MSG_COUNTDOWN_FINISHED:
		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] MSG_COUNTDOWN_FINISHED " + std::to_string(msg.senderId));
		break;

	case (int)ServerMessage::MSG_PLAYER_DEAD:
		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] MSG_PLAYER_DEAD " + std::to_string(msg.senderId));

		CMultiplayManager::GetInst()->SetPlayerIsDeadInGameFromId(msg.senderId, true);
		break;

	case (int)ServerMessage::MSG_GAME_OVER:
		CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] MSG_GAME_OVER " + msg.body.data());

		CMultiplayManager::GetInst()->SetIsGameStart(false);
		break;

	case (int)ServerMessage::MSG_MOVE_UP:
		CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_MOVE_UP");
		break;

	case (int)ServerMessage::MSG_MOVE_DOWN:
		CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_MOVE_DOWN");
		break;

	case (int)ServerMessage::MSG_PLAYER_DISTANCE:
	{
		//CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_PLAYER_DISTANCE");

		float distance;
		if (msg.body.size() >= sizeof(float))
			memcpy(&distance, msg.body.data(), sizeof(float));

		//CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_PLAYER_DISTANCE: " + std::to_string(distance));
		CMultiplayManager::GetInst()->SetPlayerDistanceInGameFromId(msg.senderId, distance);
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_HEIGHT:
	{
		//CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_PLAYER_HEIGHT");

		float height;
		if (msg.body.size() >= sizeof(float))
			memcpy(&height, msg.body.data(), sizeof(float));

		//CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_PLAYER_HEIGHT: " + std::to_string(height));
		CMultiplayManager::GetInst()->SetPlayerHeightInGameFromId(msg.senderId, height);
		break;
	}
	case (int)ServerMessage::MSG_TAKEN_DAMAGE:
	{
		//CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_TAKEN_DAMAGE");

		float curHp;
		if (msg.body.size() >= sizeof(float))
			memcpy(&curHp, msg.body.data(), sizeof(float));

		//CLog::PrintLog("[Game] Client " + std::to_string(msg.senderId) + " MSG_TAKEN_DAMAGE: " + std::to_string(curHp));
		CMultiplayManager::GetInst()->SetPlayerCurHpInGameFromId(msg.senderId, curHp);
		break;
	}
		//////////////////////기타 상시///////////////////////////////////
	default:
		if (msg.msgType != (int)ServerMessage::MSG_HEARTBEAT_ACK)
			CLog::PrintLog("[MSG " + std::to_string(msg.msgType) + "] From " + std::to_string(msg.senderId));
		break;
	}
}

void CNetworkManager::ReceiveThread(SOCKET sock)
{
	while (true)
	{
		if (!mIsConnection)
			break;

		MessageHeader header;
		std::vector<char> bodyBuffer;
		if (!ReceiveMsg(sock, header, bodyBuffer))
		{
			CLog::PrintLog("Disconnected from server.");

			// 나에게 보내는 라스트 메시지..
			std::lock_guard<std::mutex> lock(mQueueMutex);
			mMessageQueue.push({ 0, (int)ServerMessage::MSG_END, std::move(bodyBuffer) });

			break;
		}
		std::lock_guard<std::mutex> lock(mQueueMutex);
		mMessageQueue.push({ header.senderId, header.msgType, std::move(bodyBuffer) });
	}
}

void CNetworkManager::HeartbeatThread(SOCKET sock)
{
	while (true)
	{
		if (!mIsConnection)
			break;

		std::this_thread::sleep_for(std::chrono::seconds(1));
		SendMsg(0, (int)ClientMessage::MSG_HEARTBEAT, nullptr, 0);
	}
}

bool CNetworkManager::SendAll(SOCKET sock, const char* data, int len)
{
	int sent = 0;
	while (sent < len)
	{
		int r = send(sock, data + sent, len - sent, 0);
		if (r == SOCKET_ERROR) return false;
		sent += r;
	}
	return true;
}

bool CNetworkManager::ReceiveMsg(SOCKET sock, MessageHeader& header, std::vector<char>& bodyBuffer)
{
	if (!RecvAll(sock, (char*)&header, sizeof(header))) return false;
	bodyBuffer.resize(header.bodyLen);
	if (header.bodyLen > 0)
		return RecvAll(sock, bodyBuffer.data(), header.bodyLen);
	return true;
}

bool CNetworkManager::RecvAll(SOCKET sock, char* buffer, int len)
{
	int recvd = 0;
	while (recvd < len)
	{
		int r = recv(sock, buffer + recvd, len - recvd, 0);
		if (r <= 0) return false;
		recvd += r;
	}
	return true;
}



#pragma region MessageProcess

/*
while (true)
{
	std::string input;
	std::getline(std::cin, input);

	if (input == "start")
		client.SendMsg(0, (int)ClientMessage::MSG_START, nullptr, 0);
	else if (input == "ready")
		client.SendMsg(0, (int)ClientMessage::MSG_READY, nullptr, 0);
	else if (input == "unready")
		client.SendMsg(0, (int)ClientMessage::MSG_UNREADY, nullptr, 0);
	else if (input == "up")
		client.SendMsg(0, (int)ClientMessage::MSG_MOVE_UP, nullptr, 0);
	else if (input == "down")
		client.SendMsg(0, (int)ClientMessage::MSG_MOVE_DOWN, nullptr, 0);
	else if (input == "dead")
		client.SendMsg(0, (int)ClientMessage::MSG_PLAYER_DEAD, nullptr, 0);
	else if (input.rfind("map ", 0) == 0)
	{
		int mapId = std::stoi(input.substr(4));
		client.SendMsg(0, (int)ClientMessage::MSG_PICK_MAP, &mapId, sizeof(int));
	}
	else if (input.rfind("char ", 0) == 0)
	{
		int charId = std::stoi(input.substr(5));
		client.SendMsg(0, (int)ClientMessage::MSG_PICK_CHARACTER, &charId, sizeof(int));
	}
	else if (input.rfind("item ", 0) == 0)
	{
		int slot = -1, itemId = -1;
		sscanf_s(input.c_str() + 5, "%d %d", &slot, &itemId);
		int data[2] = { slot, itemId };
		client.SendMsg(0, (int)ClientMessage::MSG_PICK_ITEM, data, sizeof(data));
	}
	else
	{
		std::cout << "[Client] Unknown command.\n";
	}
}*/

/*
// 메시지 처리 루프
RecvMessage msg;
if (client.PollMessage(msg))
{
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_CONNECTED:
	{
		int id;
		memcpy(&id, msg.body.data(), sizeof(int));
		std::cout << "[System " << msg.msgType << "] Connected. My ID: " << id << "\n";
		break;
	}
	case (int)ServerMessage::MSG_NEW_OWNER:
	{
		int id;
		memcpy(&id, msg.body.data(), sizeof(int));
		std::cout << "[System " << msg.msgType << "] New Room Owner: " << id << "\n";
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_DEAD:
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " died.\n";
		break;

	case (int)ServerMessage::MSG_READY:
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " MSG_READY \n";
		break;

	case (int)ServerMessage::MSG_UNREADY:
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " MSG_UNREADY \n";
		break;

	case (int)ServerMessage::MSG_PICK_MAP:
	{
		if (msg.body.size() >= sizeof(int)) {
			int mapId;
			memcpy(&mapId, msg.body.data(), sizeof(int));
			std::cout << "[Game " << msg.msgType << "] Map changed to " << mapId << "\n";
		}
		break;
	}

	case (int)ServerMessage::MSG_PICK_ITEM:
	{
		if (msg.body.size() >= sizeof(int) * 2) {
			int slot, itemId;
			memcpy(&slot, msg.body.data(), sizeof(int));
			memcpy(&itemId, msg.body.data() + sizeof(int), sizeof(int));
			std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " picked item " << itemId << " in slot " << slot << "\n";
		}
		break;
	}

	case (int)ServerMessage::MSG_PICK_CHARACTER:
	{
		if (msg.body.size() >= sizeof(int)) {
			int characterId;
			memcpy(&characterId, msg.body.data(), sizeof(int));
			std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " picked character " << characterId << "\n";
		}
		break;
	}
	case (int)ServerMessage::MSG_CLIENT_LIST:
	{
		std::cout << "[System " << msg.msgType << "] Client list received: ";
		int count = msg.body.size() / sizeof(int);
		for (int i = 0; i < count; ++i) {
			int id;
			memcpy(&id, msg.body.data() + i * sizeof(int), sizeof(int));
			std::cout << id << " ";
		}
		std::cout << "\n";
		break;
	}
	case (int)ServerMessage::MSG_JOIN:
	{
		if (msg.body.size() >= sizeof(int)) {
			int newId;
			memcpy(&newId, msg.body.data(), sizeof(int));
			std::cout << "[System " << msg.msgType << "] New client joined: " << newId << "\n";
		}
		break;
	}
	case (int)ServerMessage::MSG_DISCONNECT:
	{
		int id;
		memcpy(&id, msg.body.data(), sizeof(int));
		std::cout << "[System " << msg.msgType << "] MSG_DISCONNECT from server. ID: " << id << "\n";
		break;
	}
	case (int)ServerMessage::MSG_CONNECTED_REJECT:
	{
		std::string reason(msg.body.begin(), msg.body.end());
		std::cout << "[System " << msg.msgType << "] MSG_CONNECTED_REJECT from server. Reason: " << reason << "\n";
		//exit(0); // 앱 종료.
		break;
	}
	case (int)ServerMessage::MSG_GAME_OVER:
		std::cout << "[Game " << msg.msgType << "] " << msg.body.data() << "\n";
		break;

	case (int)ServerMessage::MSG_MOVE_UP:
		std::cout << "[Game] Client " << msg.senderId << " moved UP\n";
		break;

	case (int)ServerMessage::MSG_MOVE_DOWN:
		std::cout << "[Game] Client " << msg.senderId << " moved DOWN\n";
		break;

	case (int)ServerMessage::MSG_INFO:
		std::cout << "[Info " << msg.msgType << "] " << msg.body.data() << "\n";
		break;

	case (int)ServerMessage::MSG_START_ACK:

		break;

	default:
		if (msg.msgType != (int)ServerMessage::MSG_HEARTBEAT_ACK)
			std::cout << "[MSG " << msg.msgType << "] From " << msg.senderId << "\n";
		break;
	}
}
*/

#pragma endregion