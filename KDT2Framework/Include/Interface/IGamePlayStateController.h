#pragma once

#include "GameInfo.h"
/// <summary>
/// 용도: 인게임 내에서 플레이 상태를 나타내기위함.
/// </summary>
class IGamePlayStateController abstract
{
private:
	EGamePlayState::Type gamePlayState;

public:
	virtual void SetGamePlayState(EGamePlayState::Type type)
	{
		gamePlayState = type;
	}

	inline EGamePlayState::Type GetGamePlayState() { return gamePlayState; };
	inline bool IsGamePlayEnableByState()
	{
		if (GetGamePlayState() == EGamePlayState::Start
			|| GetGamePlayState() == EGamePlayState::Freze)
		{
			return true;
		}
		return false;
	}
	inline bool IsPlayerControlEnableByState()
	{
		if (GetGamePlayState() == EGamePlayState::Start)
		{
			return true;
		}
		return false;
	}

};