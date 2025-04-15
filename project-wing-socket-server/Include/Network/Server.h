#pragma once
#include "GameInfo.h"

#define PORT 12345
#define MAX_PLAYERS 8

#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

// 높이 가운데서부터 시작 함.
#define PLAYER_INIT_POS_HEIGHT 0.0f

class CServer
{
public:
	CServer();
	~CServer();

private:
	SOCKET mServer;
	std::recursive_mutex mMutex;
	std::vector<struct CClient*> mClients;
	std::unordered_set<int> mDeadPlayers;
	std::map<int, Obstacle> mObstaclesByStep;

	GameState mState = WAITING;
	int mNextId = 1;
	int mRoomOwner = -1;
	int mMapId = 0;

	LARGE_INTEGER mSecond = {};
	LARGE_INTEGER mTime = {};

	float mDeltaTime = 0.f;
	float mFPS = 0.f;
	float mFPSTime = 0.f;
	int mFPSTick = 0;

private:
	void InitTimer();
	float UpdateTimer();
	float GetDeltaTimeFromTimer();

public:
	bool Init();
	void Run();

private:
	bool SendAll(SOCKET sock, const char* data, int len);
	bool SendMsg(SOCKET sock, int senderId, int msgType, const void* body, int bodyLen);
	void Broadcast(int senderId, int msgType, const void* data, int len);
	bool RecvAll(SOCKET sock, char* buffer, int len);
	bool ReceiveMsg(SOCKET sock, MessageHeader& header, std::vector<char>& body);


private:
	void CheckGameOver();
	void SendRoomFullInfo(struct CClient* client);
	void InGameUpdateLoop();
	void ClientThread(struct CClient* client);

};

