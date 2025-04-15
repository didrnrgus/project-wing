#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#pragma comment(lib, "libcurl-x64.lib")

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type)	\
private:\
	Type();\
	~Type();\
private:\
	static Type*	mInst;\
public:\
	static Type* GetInst()\
	{\
		if(!mInst)\
			mInst = new Type;\
		return mInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(mInst);\
	}

#define	DEFINITION_SINGLE(Type)	Type* Type::mInst = nullptr;

template<typename T>
const T& clamp(const T & v, const T & lo, const T & hi) {
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

enum GameState { WAITING, RUNNING };

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
		MSG_CHANGE_NICKNAME,

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
		MSG_END	// 서버에서는 쓰지 않음.
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

struct Obstacle
{
	float scale;
	float rotation;
	float height;
};