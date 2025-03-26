#pragma once
#include "NetworkManager.h"
#include "GameManager.h"

#define PORT 12345
DEFINITION_SINGLE(CNetworkManager);

CNetworkManager::CNetworkManager()
{
	mIsConnected = false;
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

	if (connect(mSock, (sockaddr*)&mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR) return false;

	CLog::PrintLog("CNetworkManager::ConnetServer()");
	mIsConnected = true;

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
	mIsConnected = false;

	if (mSock == INVALID_SOCKET)
		return;

	if (mRecvThread && mRecvThread->joinable())
		mRecvThread->join();
	if (mHbThread && mHbThread->joinable())
		mHbThread->join();

	closesocket(mSock);
	
}

void CNetworkManager::ReceiveThread(SOCKET sock)
{
	while (true)
	{
		if (!mIsConnected)
			break;

		MessageHeader header;
		std::vector<char> bodyBuffer;
		if (!ReceiveMsg(sock, header, bodyBuffer))
		{
			CLog::PrintLog("Disconnected from server.");
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
		if (!mIsConnected)
			break;

		std::this_thread::sleep_for(std::chrono::seconds(1));
		SendMsg(0, (int)ClientMessage::Type::MSG_HEARTBEAT, nullptr, 0);
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
client.SendMsg(0, (int)ClientMessage::Type::MSG_START, nullptr, 0);
client.SendMsg(0, (int)ClientMessage::Type::MSG_READY, nullptr, 0);
client.SendMsg(0, (int)ClientMessage::Type::MSG_UNREADY, nullptr, 0);
client.SendMsg(0, (int)ClientMessage::Type::MSG_MOVE_UP, nullptr, 0);
client.SendMsg(0, (int)ClientMessage::Type::MSG_MOVE_DOWN, nullptr, 0);
client.SendMsg(0, (int)ClientMessage::Type::MSG_PLAYER_DEAD, nullptr, 0);

// 여기서부터는 데이터가 껴들어감.
int mapId = std::stoi(input.substr(4));
client.SendMsg(0, (int)ClientMessage::Type::MSG_PICK_MAP, &mapId, sizeof(int));

int characterId = std::stoi(input.substr(5));
client.SendMsg(0, (int)ClientMessage::Type::MSG_PICK_CHARACTER, &characterId, sizeof(int));

int data[2] = { slot, itemId };
client.SendMsg(0, (int)ClientMessage::Type::MSG_PICK_ITEM, data, sizeof(data));
*/

/*
// 메시지 처리 루프
RecvMessage msg;
if (client.PollMessage(msg))
{
	switch (msg.msgType)
	{
	case (int)ServerMessage::Type::MSG_CONNECTED:
	{
		int id;
		memcpy(&id, msg.body.data(), sizeof(int));
		std::cout << "[System " << msg.msgType << "] Connected. My ID: " << id << "\n";
		break;
	}
	case (int)ServerMessage::Type::MSG_NEW_OWNER:
	{
		int id;
		memcpy(&id, msg.body.data(), sizeof(int));
		std::cout << "[System " << msg.msgType << "] New Room Owner: " << id << "\n";
		break;
	}
	case (int)ServerMessage::Type::MSG_PLAYER_DEAD:
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " died.\n";
		break;

	case (int)ServerMessage::Type::MSG_READY:
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " MSG_READY \n";
		break;

	case (int)ServerMessage::Type::MSG_UNREADY:
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " MSG_UNREADY \n";
		break;

	case (int)ServerMessage::Type::MSG_PICK_MAP:
	{
		if (msg.body.size() >= sizeof(int)) {
			int mapId;
			memcpy(&mapId, msg.body.data(), sizeof(int));
			std::cout << "[Game " << msg.msgType << "] Map changed to " << mapId << "\n";
		}
		break;
	}

	case (int)ServerMessage::Type::MSG_PICK_ITEM:
	{
		if (msg.body.size() >= sizeof(int) * 2) {
			int slot, itemId;
			memcpy(&slot, msg.body.data(), sizeof(int));
			memcpy(&itemId, msg.body.data() + sizeof(int), sizeof(int));
			std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " picked item " << itemId << " in slot " << slot << "\n";
		}
		break;
	}

	case (int)ServerMessage::Type::MSG_PICK_CHARACTER:
	{
		if (msg.body.size() >= sizeof(int)) {
			int characterId;
			memcpy(&characterId, msg.body.data(), sizeof(int));
			std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " picked character " << characterId << "\n";
		}
		break;
	}
	case (int)ServerMessage::Type::MSG_CLIENT_LIST:
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
	case (int)ServerMessage::Type::MSG_JOIN:
	{
		if (msg.body.size() >= sizeof(int)) {
			int newId;
			memcpy(&newId, msg.body.data(), sizeof(int));
			std::cout << "[System " << msg.msgType << "] New client joined: " << newId << "\n";
		}
		break;
	}
	case (int)ServerMessage::Type::MSG_DISCONNECT:
	{
		int id;
		memcpy(&id, msg.body.data(), sizeof(int));
		std::cout << "[System " << msg.msgType << "] MSG_DISCONNECT from server. ID: " << id << "\n";
		break;
	}
	case (int)ServerMessage::Type::MSG_CONNECTED_REJECT:
	{
		std::string reason(msg.body.begin(), msg.body.end());
		std::cout << "[System " << msg.msgType << "] MSG_CONNECTED_REJECT from server. Reason: " << reason << "\n";
		//exit(0); // 앱 종료.
		break;
	}
	case (int)ServerMessage::Type::MSG_GAME_OVER:
		std::cout << "[Game " << msg.msgType << "] " << msg.body.data() << "\n";
		break;

	case (int)ServerMessage::Type::MSG_MOVE_UP:
		std::cout << "[Game] Client " << msg.senderId << " moved UP\n";
		break;

	case (int)ServerMessage::Type::MSG_MOVE_DOWN:
		std::cout << "[Game] Client " << msg.senderId << " moved DOWN\n";
		break;

	case (int)ServerMessage::Type::MSG_INFO:
		std::cout << "[Info " << msg.msgType << "] " << msg.body.data() << "\n";
		break;

	case (int)ServerMessage::Type::MSG_START_ACK:

		break;

	default:
		if (msg.msgType != (int)ServerMessage::Type::MSG_HEARTBEAT_ACK)
			std::cout << "[MSG " << msg.msgType << "] From " << msg.senderId << "\n";
		break;
	}
}
*/

#pragma endregion