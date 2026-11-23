#pragma once

// 게임 데이터 JSON(GitHub Pages)을 받아 서버 계산에 필요한 값만 보관한다.
// 클라이언트 KDT2Framework/Include/Etc/JsonController.cpp 와 같은 키를 읽는다.

#include <string>
#include <map>

#define WEBSERVER_HOST	L"didrnrgus.github.io"
#define WEBSERVER_DIR	L"/project-wing-webserver/"

namespace EStatType
{
	// 클라이언트 EStatInfoText 와 같은 순서 (item_list.json 의 stat_type)
	enum Type
	{
		HP = 0,
		Speed,
		Dex,
		Def,
		End
	};
}

struct FCharacterStat
{
	float HP = 100.0f;
	float Speed = 700.0f;
	float Dex = 100.0f;
	float Def = 10.0f;
	float StunDuration = 2.0f;
};

struct FItemStat
{
	int StatType = EStatType::End;
	float AddValue = 0.0f;
};

struct FMapStat
{
	float CollisionDamage = 20.0f;
	float ObstacleIntervalTime = 2.0f;
};

class CGameData
{
public:
	// 실패해도 false 만 반환하고 기본값으로 동작한다.
	bool Load();
	bool IsLoaded() const { return mIsLoaded; }

	const FCharacterStat& GetCharacter(int _index) const;
	const FItemStat* GetItem(int _index) const;
	const FMapStat& GetMap(int _index) const;
	int GetMapCount() const { return (int)mMaps.size(); }

private:
	std::map<int, FCharacterStat> mCharacters;
	std::map<int, FItemStat> mItems;
	std::map<int, FMapStat> mMaps;
	bool mIsLoaded = false;

	FCharacterStat mDefaultCharacter;
	FMapStat mDefaultMap;
};
