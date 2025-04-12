#pragma once

#include "GameInfo.h"

extern std::string gIPAddress;

namespace ClientMessage
{
	enum Type
	{
		MSG_HEARTBEAT,
		MSG_WHO_ARE_YOU_RES,
		MSG_START,
		MSG_PICK_CHARACTER,
		MSG_PICK_ITEM,
		MSG_PICK_MAP,
		MSG_READY,
		MSG_UNREADY,
		MSG_MOVE_UP,
		MSG_MOVE_DOWN,
		MSG_TAKE_DAMAGE, // 맵에 박았을때의 트리거
		MSG_BOOST_ON,
		MSG_BOOST_OFF,
		MSG_CHANGE_NICKNAME
	};
}

namespace ServerMessage
{
	enum Type
	{
		MSG_WHO_ARE_YOU_REQ,
		MSG_CONNECTED,
		MSG_ROOM_FULL_INFO,
		MSG_DISCONNECT, // 이건 누가 나간거.
		MSG_CONNECTED_REJECT,
		MSG_NEW_OWNER,
		MSG_JOIN,

		MSG_PICK_MAP,
		MSG_PICK_ITEM,
		MSG_PICK_CHARACTER,
		MSG_READY,
		MSG_UNREADY,
		MSG_START_ACK,

		MSG_COUNTDOWN_FINISHED,
		MSG_PLAYER_DEAD,
		MSG_GAME_OVER,
		MSG_MOVE_UP,
		MSG_MOVE_DOWN,
		MSG_PLAYER_DISTANCE, // 거리 전송 메시지.
		MSG_PLAYER_HEIGHT,
		MSG_TAKEN_DAMAGE,	// 현재 HP 알려줌.
		MSG_TAKEN_STUN,
		MSG_BOOST_ON,
		MSG_BOOST_OFF,
		MSG_OBSTACLE,

		MSG_HEARTBEAT_ACK,
		MSG_END
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
	bool mIsConnection;			// 커넥션 시도부터 true.
	bool mIsConnectCompleted;	// 커넥션 메시지 받고나서야 true.
	bool mIsMultiplay;			// title scene 에서 결정 됨.

public:
	bool ConnetServer();
	void SendMsg(int senderId, int msgType, const void* body, int bodyLen);
	
	// false 를 리턴할때까지 반복해서 돌려라.
	bool PollMessage(RecvMessage& out);	
	void Clear(std::function<void()>&& Func);
	bool IsConnection() { return mIsConnection; }
	bool IsConnectCompleted() { return mIsConnectCompleted; }

	void SetIsMultiplay(bool _isMulti) { mIsMultiplay = _isMulti; }
	bool IsMultiplay() { return mIsMultiplay; }

private:
	void ReceiveThread(SOCKET sock);
	void HeartbeatThread(SOCKET sock);
	bool SendAll(SOCKET sock, const char* data, int len);
	bool ReceiveMsg(SOCKET sock, MessageHeader& header, std::vector<char>& bodyBuffer);
	bool RecvAll(SOCKET sock, char* buffer, int len);
	void Clear();

	// 프레임워크 제한받지않고 메시지 받음.
	void ProcessMessage(const struct RecvMessage& msg);
	
	DECLARE_SINGLE(CNetworkManager);
};

