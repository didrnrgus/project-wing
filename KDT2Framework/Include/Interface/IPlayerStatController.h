#pragma once

#include "GameInfo.h"

// 인게임 플레이어 오브젝트에 붙음.
class IPlayerStatController abstract
{
private:
	int index;
	float maxHp;
	float curHp;
	float speed;
	float dex;

public:
	bool Init(float _maxHp, float _speed, float _dex)
	{
		maxHp = _maxHp;
		curHp = maxHp;
		speed = _speed;
		dex = _dex;
	}

	void SetIndex(int _index) { index = _index; }
	void Damaged(int _damageVal) { curHp -= _damageVal; }
	void AddSpeed(int _addSpeedVal) { speed += _addSpeedVal; }
	void AddDex(int _addDexVal) { dex += _addDexVal; }

	int GetIndex() { return index; }
	float GetMaxHP() { return maxHp; }
	float GetCurHP() { return curHp; }
	float GetSpeed() { return speed; }
	float GetDex() { return dex; }
};
