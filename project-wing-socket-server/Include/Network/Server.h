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
	SOCKET server;
	std::recursive_mutex gMutex;
	std::vector<struct CClient*> gClients;
	std::unordered_set<int> gDeadPlayers;
	std::map<int, Obstacle> gObstaclesByStep;

	GameState gState = WAITING;
	int gNextId = 1;
	int gRoomOwner = -1;
	int gMapId = 0;

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
	bool sendAll(SOCKET sock, const char* data, int len);
	bool sendMessage(SOCKET sock, int senderId, int msgType, const void* body, int bodyLen);
	void broadcast(int senderId, int msgType, const void* data, int len);
	bool recvAll(SOCKET sock, char* buffer, int len);
	bool receiveMessage(SOCKET sock, MessageHeader& header, std::vector<char>& body);


private:
	void checkGameOver();
	void sendRoomFullInfo(struct CClient* client);
	void InGameUpdateLoop();
	void clientThread(struct CClient* client);

};

