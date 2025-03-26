#pragma once

#include "GameInfo.h"

extern std::string gIPAddress;

namespace ClientMessage {
	enum class Type {
		MSG_HEARTBEAT,
		MSG_START,
		MSG_PICK_CHARACTER,
		MSG_PICK_ITEM,
		MSG_PICK_MAP,
		MSG_READY,
		MSG_UNREADY,
		MSG_MOVE_UP,
		MSG_MOVE_DOWN,
		MSG_PLAYER_DEAD
	};
}

namespace ServerMessage {
	enum class Type {
		MSG_CONNECTED,
		MSG_HEARTBEAT_ACK,
		MSG_START_ACK,
		MSG_JOIN,
		MSG_DISCONNECT,
		MSG_CONNECTED_REJECT,
		MSG_INFO,
		MSG_NEW_OWNER,
		MSG_CLIENT_LIST,
		MSG_PICK_CHARACTER,
		MSG_PICK_ITEM,
		MSG_PICK_MAP,
		MSG_READY,
		MSG_UNREADY,
		MSG_MOVE_UP,
		MSG_MOVE_DOWN,
		MSG_PLAYER_DEAD,
		MSG_GAME_OVER
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

class CNetworkManager
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
	void ProcessMessage(struct RecvMessage& msg);
	
	DECLARE_SINGLE(CNetworkManager);
};

