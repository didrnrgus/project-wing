#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

// 인게임 플레이어 오브젝트에 붙음.
// 인게임 플레이 중 변동되는 동적스탯 요소.
class IPlayerStatController abstract
{
private:
	int index;
	float maxHp;
	float baseSpeed;
	float baseDex;

	float curHp;
	float addedSpeed;
	float addedDex;

	bool isStun; // 박았을때 잠시 멈춰야 해서.

public:
	bool Init(FCharacterState stat)
	{
		return Init(stat.HP, stat.Speed, stat.Dex);
	}

	bool Init(float _maxHp, float _speed, float _dex)
	{
		maxHp = _maxHp;
		baseSpeed = _speed;
		baseDex = _dex;

		curHp = maxHp;
		addedSpeed = 0.0f;
		addedDex = 0.0f;
		return true;
	}

	inline void SetIndex(int _index) { index = _index; }
	inline void Damaged(int _damageVal) { curHp -= _damageVal; }
	inline void AddSpeed(int _addSpeedVal) { addedSpeed += _addSpeedVal; }
	inline void AddDex(int _addDexVal) { addedDex += _addDexVal; }
	inline void SetStun(bool _isStun) { isStun = _isStun; }

	inline int GetIndex() { return index; }
	inline float GetMaxHP() { return maxHp; }
	inline float GetCurHP() { return curHp; }
	inline float GetSpeed() { return baseSpeed + addedSpeed; }
	inline float GetDex() { return baseDex +  addedDex; }
	inline bool GetIsDeath() { return GetCurHP() > 0.0f; }
	inline bool GetIsStun() { return isStun; }
};
