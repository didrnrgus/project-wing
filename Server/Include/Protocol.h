#pragma once

// 클라이언트 KDT2Framework/Include/Etc/NetworkManager.h 와 순서/값이 반드시 같아야 한다.
// enum 중간에 끼워 넣지 말고 끝에만 추가할 것.

#include <cstdint>

#define SERVER_PORT						12345
#define PLAYER_COUNT_MAX				5
#define PLAYER_ITEM_COUNT_MAX			3
#define PLAYER_ITEM_TYPE_DEFAULT_INDEX	-1
#define NICKNAME_SIZE					64

namespace ClientMessage
{
	enum Type
	{
		MSG_HEARTBEAT,
		MSG_WHO_ARE_YOU_RES,	// body: char[] 닉네임 (널 포함)
		MSG_START,
		MSG_PICK_CHARACTER,		// body: int
		MSG_PICK_ITEM,			// body: int slot, int item
		MSG_PICK_MAP,			// body: int
		MSG_READY,
		MSG_UNREADY,
		MSG_MOVE_UP,
		MSG_MOVE_DOWN,
		MSG_TAKE_DAMAGE,		// body: float 맵 충돌 데미지
		MSG_BOOST_ON,
		MSG_BOOST_OFF,
		MSG_CHANGE_NICKNAME		// body: char[] 닉네임 (널 없음)
	};
}

namespace ServerMessage
{
	enum Type
	{
		MSG_WHO_ARE_YOU_REQ,	// body: int id
		MSG_CONNECTED,			// body: int id
		MSG_ROOM_FULL_INFO,		// body: 아래 WriteRoomFullInfo 참고
		MSG_DISCONNECT,			// body: int id
		MSG_CONNECTED_REJECT,	// body: char[] 사유
		MSG_NEW_OWNER,			// body: int id
		MSG_JOIN,				// body: int id + char[] 닉네임 (널 없음)

		MSG_PICK_MAP,			// body: int
		MSG_PICK_ITEM,			// body: int slot, int item
		MSG_PICK_CHARACTER,		// body: int
		MSG_READY,
		MSG_UNREADY,
		MSG_START_ACK,			// body: int 1(시작) / 0
		MSG_CHANGE_NICKNAME,	// body: char[] 닉네임 (널 없음)

		MSG_COUNTDOWN_FINISHED,
		MSG_PLAYER_DEAD,
		MSG_GAME_OVER,			// body: char[] 로그 문자열
		MSG_MOVE_UP,
		MSG_MOVE_DOWN,
		MSG_PLAYER_DISTANCE,	// body: float 거리(m)
		MSG_PLAYER_HEIGHT,		// body: float 높이(월드 y)
		MSG_TAKEN_DAMAGE,		// body: float 현재 HP
		MSG_TAKEN_STUN,
		MSG_BOOST_ON,
		MSG_BOOST_OFF,
		MSG_OBSTACLE,			// body: float 크기, float 회전, float 높이

		MSG_HEARTBEAT_ACK,
		MSG_END					// 클라이언트 내부 전용. 서버는 보내지 않는다.
	};
}

#pragma pack(push, 1)
struct MessageHeader
{
	int32_t senderId;
	int32_t msgType;
	int32_t bodyLen;
};
#pragma pack(pop)

static_assert(sizeof(MessageHeader) == 12, "MessageHeader must be 12 bytes");
