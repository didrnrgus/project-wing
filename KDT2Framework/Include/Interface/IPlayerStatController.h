﻿#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

// 인게임 플레이어 오브젝트에 붙음.
// 인게임 플레이 중 변동되는 동적스탯 요소.
class IPlayerStatController abstract
{
private:
	//int index;
	float maxHp;
	float baseSpeed;
	float baseDex;
	float baseDef;
	float stunDuration;

	float curHp;
	float addedSpeed;
	float addedDex;
	float addedDef;

	float addedReleaseStunValue;
	float addedReleaseProtectionValue;

	bool isStun; // 박았을때 잠시 멈춰야 해서.
	bool isProtection;
	bool isBoostMode;
	float playDistance;

	std::function<void()> playerFrezeCallback;

private:
	bool InitStat(float _maxHp, float _speed, float _dex, float _def, float _stun)
	{
		maxHp = _maxHp;
		baseSpeed = _speed;
		baseDex = _dex;
		baseDef = _def;
		stunDuration = _stun;

		curHp = maxHp;
		addedSpeed = 0.0f;
		addedDex = 0.0f;
		addedDef = 0.0f;

		addedReleaseStunValue = 0.0f;
		addedReleaseProtectionValue = 0.0f;

		isStun = false;
		isProtection = false;
		isBoostMode = false;
		playDistance = 0.0f;

		return true;
	}

public:
	bool InitStat(FCharacterState stat)
	{
		return InitStat(stat.HP, stat.Speed, stat.Dex, stat.Def, stat.StunDuration);
	}

	//void SetIndex(int _index) { index = _index; }
	void AddValueByStatIndex(EStatInfoText::Type _statIndex, float _value)
	{
		switch (_statIndex)
		{
		case EStatInfoText::HP:
			AddHp(_value);
			break;
		case EStatInfoText::Speed:
			AddSpeed(_value);
			break;
		case EStatInfoText::Dex:
			AddDex(_value);
			break;
		case EStatInfoText::Def:
			AddDef(_value);
			break;
		default:
			break;
		}
	}
	inline void Damaged(float _damageVal)
	{
		float result = _damageVal - GetDef();
		result = result < 0.0f ? 0.0f : result;
		curHp -= result;

		if (curHp <= 0.0f && playerFrezeCallback != nullptr)
		{
			curHp = 0.0f;
			playerFrezeCallback();
			playerFrezeCallback = nullptr;
		}
	}
	inline void DamagedPerDistance(float _damageVal)
	{
		float result = _damageVal - (_damageVal * (GetDef() * 0.01f));
		result = result < 0.0f ? 0.0f : result;
		curHp -= result;

		if (curHp <= 0.0f && playerFrezeCallback != nullptr)
		{
			curHp = 0.0f;
			playerFrezeCallback();
			playerFrezeCallback = nullptr;
		}
	}
	inline void AddHp(float _addHp)
	{
		curHp += _addHp;

		if (curHp > maxHp)
			maxHp = curHp;
	}
	inline void AddSpeed(float _addSpeedVal) { addedSpeed += _addSpeedVal; }
	inline void AddDex(float _addDexVal) { addedDex += _addDexVal; }
	inline void AddDef(float _addDefVal) { addedDef += _addDefVal; }
	inline void AddPlayDistance(float _addDist) { playDistance += _addDist; }
	inline void SetPlayDistance(float _dist) { playDistance = _dist; }
	inline void SetCurHp(const float _curHp) 
	{
		curHp = _curHp; 

		if (curHp <= 0.0f && playerFrezeCallback != nullptr)
		{
			curHp = 0.0f;
			playerFrezeCallback();
			playerFrezeCallback = nullptr;
		}
	}
	inline void SetStun() { isStun = true; }
	inline void SetIsBoostMode(const bool _isBoostMode) { isBoostMode = _isBoostMode; }
	inline void ReleaseStun(float DeltaTime)
	{
		addedReleaseStunValue += DeltaTime;
		if (addedReleaseStunValue > stunDuration)
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

	//inline int GetIndex() { return index; }
	inline float GetMaxHP() { return maxHp; }
	inline float GetCurHP() { return curHp; }
	inline float GetSpeed() { return baseSpeed + addedSpeed; }
	inline float GetDex() { return baseDex + addedDex; }
	inline float GetDef() { return baseDef + addedDef; }
	inline float GetStunDuration() { return stunDuration; }
	inline bool GetIsDeath() { return GetCurHP() > 0.0f; }
	inline bool GetIsStun() { return isStun; }
	inline bool GetIsProtection() { return isProtection; }
	inline float GetBoostValue() { return isBoostMode ? 2.0f : 1.0f; }
	inline float GetPlayDistance() { return playDistance; }

protected:
	// 콜백 함수들 등록
	template <typename T>
	void SetPlayerFrezeCallback(T* Obj, void(T::* Func)())
	{
		playerFrezeCallback = std::bind(Func, Obj);
	}

	// 람다 넣으려고.
	using T = std::function<void()>;
	void SetPlayerFrezeCallback(T&& Func)
	{
		playerFrezeCallback = std::move(Func);
	}
};
