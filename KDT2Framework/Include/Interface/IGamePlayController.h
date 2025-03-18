#pragma once

#include "GameInfo.h"
/// <summary>
/// 용도: 인게임 내에서 플레이 상태를 나타내기위함.
/// </summary>

namespace EGamePlayState
{
	enum Type
	{
		Ready,
		ReadyCount,
		Start,
		Pause,	// 로컬에서 자기자신만 가능. -> 인게임중 팝업?
		Exit,	// 로컬에서 자기자신만 가능. -> 인게임중 나가버림.
		End		// 정상종료
	};
}

class IGamePlayController abstract
{
private:
	EGamePlayState::Type gamePlayState;

public:
	virtual void SetGamePlayState(EGamePlayState::Type type)
	{
		gamePlayState = type;
	}

	EGamePlayState::Type GetGamePlayState() { return gamePlayState; };
};