#pragma once

#include "GameInfo.h"

extern std::string gIPAddress;

namespace ClientMessage {
	enum class Type {
		MSG_HEARTBEAT,			// 0
		MSG_START,				// 1
		MSG_PICK_CHARACTER,		// 2
		MSG_PICK_ITEM,			// 3
		MSG_PICK_MAP,			// 4
		MSG_READY,				// 5
		MSG_UNREADY,			// 6
		MSG_MOVE_UP,			// 7
		MSG_MOVE_DOWN,			// 8
		MSG_PLAYER_DEAD			// 9
	};
}

namespace ServerMessage {
	enum class Type {
		MSG_CONNECTED,		// 0
		MSG_HEARTBEAT_ACK,	// 1
		MSG_START_ACK,		// 2
		MSG_JOIN,			// 3
		MSG_DISCONNECT,		// 4
		MSG_INFO,			// 5
		MSG_NEW_OWNER,		// 6
		MSG_CLIENT_LIST,	// 7
		MSG_PICK_CHARACTER,	// 8
		MSG_PICK_ITEM,		// 9
		MSG_PICK_MAP,		// 10
		MSG_READY,			// 11
		MSG_UNREADY,		// 12
		MSG_MOVE_UP,		// 13
		MSG_MOVE_DOWN,		// 14
		MSG_PLAYER_DEAD,	// 15
		MSG_GAME_OVER		// 16
	};
}

#pragma pack(push, 1)
struct MessageHeader
{
	int senderId;
	int msgType;
	int bodyLen;
};
#pragma pack(pop)

struct RecvMessage
{
	int senderId;
	int msgType;
	std::vector<char> body;
};

class CMultiplayManager
{
private:
	WSADATA mWsaData;
	SOCKET mSock;
	sockaddr_in mServerAddr;

	std::unique_ptr<std::thread> mRecvThread;
	std::unique_ptr<std::thread> mHbThread;

	std::mutex mQueueMutex;
	std::queue<RecvMessage> mMessageQueue;
	bool mIsConnected;
	bool mIsMultiplay;	// title scene 에서 결정 됨.

public:
	bool ConnetServer();
	void SendMsg(int senderId, int msgType, const void* body, int bodyLen);
	bool PollMessage(RecvMessage& out);
	void Clear(std::function<void()>&& Func);
	bool IsConnected() { return mIsConnected; }

	void SetIsMultiplay(bool _isMulti) { mIsMultiplay = _isMulti; }
	bool IsMultiplay() { return mIsMultiplay; }

private:
	void ReceiveThread(SOCKET sock);
	void HeartbeatThread(SOCKET sock);
	bool SendAll(SOCKET sock, const char* data, int len);
	bool ReceiveMsg(SOCKET sock, MessageHeader& header, std::vector<char>& bodyBuffer);
	bool RecvAll(SOCKET sock, char* buffer, int len);
	void Clear();
	
	DECLARE_SINGLE(CMultiplayManager);
};

