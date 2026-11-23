#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <memory>
#include <random>
#include <string>
#include <vector>

#include "GameData.h"
#include "Protocol.h"

// 서버 상수. 클라이언트 동작에 맞춘 값이다.
#define TICK_INTERVAL_SEC		0.05f	// 게임 틱 / 상태 동기화 주기
#define COUNTDOWN_SEC			7.5f	// START_ACK 후 COUNTDOWN_FINISHED 까지. 클라 씬 로딩 2초 + "Ready,3,2,1,Start" 5초
#define PROTECTION_SEC			2.0f	// 스턴 후 무적 시간 (클라 ReleaseProtection)
#define HEARTBEAT_TIMEOUT_SEC	10.0f	// 이 시간 동안 아무것도 안 오면 끊는다 (클라는 1초마다 하트비트)
#define REJECT_LINGER_SEC		2.0f	// 거절 후 상대가 끊기를 기다리는 최대 시간
#define BOOST_MULTIPLY			2.0f
#define MAX_BODY_LEN			4096

namespace ERoomPhase
{
	enum Type
	{
		Lobby,
		Countdown,
		Playing
	};
}

struct FClient
{
	SOCKET sock = INVALID_SOCKET;
	int id = 0;
	bool joined = false;		// WHO_ARE_YOU_RES 처리 후 방에 들어온 상태
	bool closing = false;		// 이번 루프 끝에 정리
	std::string nickname;
	std::vector<char> recvBuf;
	std::vector<char> sendBuf;
	double lastRecvTime = 0.0;
	bool rejected = false;		// 거절 메시지를 보내고 송신만 닫은 상태. 상대가 끊을 때까지 수신은 버린다.
	double rejectTime = 0.0;

	// 로비
	bool ready = false;
	int character = 0;
	int items[PLAYER_ITEM_COUNT_MAX] = { PLAYER_ITEM_TYPE_DEFAULT_INDEX, PLAYER_ITEM_TYPE_DEFAULT_INDEX, PLAYER_ITEM_TYPE_DEFAULT_INDEX };

	// 인게임
	bool inGame = false;
	bool dead = false;
	bool movingUp = false;
	bool boost = false;
	float hp = 0.0f;
	float speed = 0.0f;
	float dex = 0.0f;
	float def = 0.0f;
	float stunDuration = 0.0f;
	float distance = 0.0f;
	float height = 0.0f;
	float stunTimer = 0.0f;
	float protectTimer = 0.0f;

	void ResetLobbyState();
};

class CGameServer
{
public:
	bool Init(unsigned short _port);
	void Run();
	void Stop() { mRunning = false; }
	void Shutdown();

private:
	SOCKET mListenSock = INVALID_SOCKET;
	std::vector<std::unique_ptr<FClient>> mClients;	// 접속 순서 유지 (방장 위임 순서)
	volatile bool mRunning = false;

	int mNextId = 1;
	int mOwnerId = -1;
	int mMapId = 0;
	ERoomPhase::Type mPhase = ERoomPhase::Lobby;
	float mCountdownTimer = 0.0f;
	float mObstacleTimer = 0.0f;

	CGameData mGameData;
	std::mt19937 mRng{ std::random_device{}() };

private:
	double Now() const;

	// 네트워크
	void AcceptClient();
	void ReceiveFrom(FClient& _client);
	void FlushAll();
	bool Flush(FClient& _client);
	void RemoveClosedClients();
	void CheckTimeouts();

	void Send(FClient& _client, int _senderId, int _msgType, const void* _body = nullptr, int _bodyLen = 0);
	void Broadcast(int _senderId, int _msgType, const void* _body = nullptr, int _bodyLen = 0, const FClient* _exclude = nullptr);
	void SendInt(FClient& _client, int _senderId, int _msgType, int _value);
	void BroadcastInt(int _senderId, int _msgType, int _value);
	void BroadcastFloat(int _senderId, int _msgType, float _value);

	// 메시지 처리
	void HandleMessage(FClient& _client, const MessageHeader& _header, const char* _body);
	void HandleWhoAreYouRes(FClient& _client, const char* _body, int _len);
	void HandleStart(FClient& _client);
	void HandleTakeDamage(FClient& _client, float _damage);
	void Reject(FClient& _client, const std::string& _reason);
	void WriteRoomFullInfo(std::vector<char>& _out) const;

	// 게임
	void Tick(float _deltaTime);
	void StartGame();
	void InitPlayerStat(FClient& _client);
	void KillPlayer(FClient& _client, const char* _reason);
	void CheckGameOver();
	void EndGame(const std::string& _reason);
	void ResetRoom();

	FClient* FindClient(int _id);
	int GetJoinedCount() const;
};
