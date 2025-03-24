#pragma once
#include "MultiplayManager.h"
#include "GameManager.h"

#define PORT 12345
DEFINITION_SINGLE(CMultiplayManager);

/*
	client.SendMsg(0, (int)ClientMessage::Type::MSG_START, nullptr, 0);
	client.SendMsg(0, (int)ClientMessage::Type::MSG_READY, nullptr, 0);
	client.SendMsg(0, (int)ClientMessage::Type::MSG_UNREADY, nullptr, 0);
	client.SendMsg(0, (int)ClientMessage::Type::MSG_MOVE_UP, nullptr, 0);
	client.SendMsg(0, (int)ClientMessage::Type::MSG_MOVE_DOWN, nullptr, 0);
	client.SendMsg(0, (int)ClientMessage::Type::MSG_PLAYER_DEAD, nullptr, 0);
	int mapId = std::stoi(input.substr(4));
	client.SendMsg(0, (int)ClientMessage::Type::MSG_PICK_MAP, &mapId, sizeof(int));
	int characterId = std::stoi(input.substr(5));
	client.SendMsg(0, (int)ClientMessage::Type::MSG_PICK_CHARACTER, &characterId, sizeof(int));
	int data[2] = { slot, itemId };
	client.SendMsg(0, (int)ClientMessage::Type::MSG_PICK_ITEM, data, sizeof(data));
*/

/*
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
		std::cout << "[Game " << msg.msgType << "] Player " << msg.senderId << " died.\n"; break;
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

	case (int)ServerMessage::Type::MSG_GAME_OVER:
		std::cout << "[Game " << msg.msgType << "] " << msg.body.data() << "\n"; break;
	case (int)ServerMessage::Type::MSG_MOVE_UP:
		std::cout << "[Game] Client " << msg.senderId << " moved UP\n"; break;

	case (int)ServerMessage::Type::MSG_MOVE_DOWN:
		std::cout << "[Game] Client " << msg.senderId << " moved DOWN\n"; break;
	case (int)ServerMessage::Type::MSG_INFO:
		std::cout << "[Info " << msg.msgType << "] " << msg.body.data() << "\n"; break;
	default:
		if (msg.msgType != (int)ServerMessage::Type::MSG_HEARTBEAT_ACK)
			std::cout << "[MSG " << msg.msgType << "] From " << msg.senderId << "\n";
		break;
	}
}
std::this_thread::sleep_for(std::chrono::milliseconds(10));
*/

CMultiplayManager::CMultiplayManager()
{
	mIsConnected = false;
}

CMultiplayManager::~CMultiplayManager()
{
	Clear();
	WSACleanup();
}

bool CMultiplayManager::ConnetServer()
{
	if (WSAStartup(MAKEWORD(2, 2), &mWsaData) != 0) return false;

	mSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSock == INVALID_SOCKET) return false;

	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_port = htons(PORT);

	inet_pton(AF_INET, gIPAddress.c_str(), &mServerAddr.sin_addr);

	if (connect(mSock, (sockaddr*)&mServerAddr, sizeof(mServerAddr)) == SOCKET_ERROR) return false;

	CLog::PrintLog("CMultiplayManager::ConnetServer()");
	mIsConnected = true;

	mRecvThread = std::make_unique<std::thread>(&CMultiplayManager::ReceiveThread, this, mSock);
	mHbThread = std::make_unique<std::thread>(&CMultiplayManager::HeartbeatThread, this, mSock);

	return true;
}

void CMultiplayManager::SendMsg(int senderId, int msgType, const void* body, int bodyLen)
{
	MessageHeader header{ senderId, msgType, bodyLen };
	SendAll(mSock, (char*)&header, sizeof(header));
	if (body && bodyLen > 0) SendAll(mSock, (char*)body, bodyLen);
}

bool CMultiplayManager::PollMessage(RecvMessage& out)
{
	std::lock_guard<std::mutex> lock(mQueueMutex);
	if (mMessageQueue.empty()) return false;
	out = std::move(mMessageQueue.front());
	mMessageQueue.pop();
	return true;
}

void CMultiplayManager::Clear(std::function<void()>&& Func)
{
	Clear();

	if (Func != nullptr)
	{
		Func();
	}
}

void CMultiplayManager::Clear()
{
	CLog::PrintLog("CMultiplayManager::Clear()");
	mIsConnected = false;

	if (mSock == INVALID_SOCKET)
		return;

	if (mRecvThread && mRecvThread->joinable())
		mRecvThread->join();
	if (mHbThread && mHbThread->joinable())
		mHbThread->join();

	closesocket(mSock);
	
}

void CMultiplayManager::ReceiveThread(SOCKET sock)
{
	while (true)
	{
		if (mIsConnected)
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

void CMultiplayManager::HeartbeatThread(SOCKET sock)
{
	while (true)
	{
		if (mIsConnected)
			break;

		std::this_thread::sleep_for(std::chrono::seconds(1));
		SendMsg(0, (int)ClientMessage::Type::MSG_HEARTBEAT, nullptr, 0);
	}
}

bool CMultiplayManager::SendAll(SOCKET sock, const char* data, int len)
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

bool CMultiplayManager::ReceiveMsg(SOCKET sock, MessageHeader& header, std::vector<char>& bodyBuffer)
{
	if (!RecvAll(sock, (char*)&header, sizeof(header))) return false;
	bodyBuffer.resize(header.bodyLen);
	if (header.bodyLen > 0)
		return RecvAll(sock, bodyBuffer.data(), header.bodyLen);
	return true;
}

bool CMultiplayManager::RecvAll(SOCKET sock, char* buffer, int len)
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