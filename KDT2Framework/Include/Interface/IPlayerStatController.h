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
	float addedReleaseStunValue;
	float addedReleaseProtectionValue;

	bool isStun; // 박았을때 잠시 멈춰야 해서.
	bool isProtection;

	// 스탯에 방어력 추가

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

		isStun = false;
		isProtection = false;
		return true;
	}

	inline void SetIndex(int _index) { index = _index; }
	inline void Damaged(float _damageVal) { curHp -= _damageVal; }
	inline void AddSpeed(float _addSpeedVal) { addedSpeed += _addSpeedVal; }
	inline void AddDex(float _addDexVal) { addedDex += _addDexVal; }
	inline void SetStun() { isStun = true; }
	inline void ReleaseStun(float DeltaTime)
	{
		addedReleaseStunValue += DeltaTime;
		if (addedReleaseStunValue > 1.0f)
		{
			addedReleaseStunValue = 0.0f;
			isStun = false;
			isProtection = true;
		}
	}
	inline void ReleaseProtection(float DeltaTime)
	{
		addedReleaseProtectionValue += DeltaTime;
		if (addedReleaseProtectionValue > 2.0f)
		{
			addedReleaseProtectionValue = 0.0f;
			isProtection = false;
		}
	}

	inline int GetIndex() { return index; }
	inline float GetMaxHP() { return maxHp; }
	inline float GetCurHP() { return curHp; }
	inline float GetSpeed() { return baseSpeed + addedSpeed; }
	inline float GetDex() { return baseDex +  addedDex; }
	inline bool GetIsDeath() { return GetCurHP() > 0.0f; }
	inline bool GetIsStun() { return isStun; }
	inline bool GetIsProtection() { return isProtection; }
};
