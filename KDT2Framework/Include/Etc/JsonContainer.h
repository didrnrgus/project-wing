#pragma once

#include "GameInfo.h"
#include "Etc/ConstValues.h"

struct FUserInfo
{
	std::string Name = "";
	std::string Map = "";
	int Distance = 0; // Rank
	int Time = 0;

	FUserInfo() {};
	FUserInfo(std::string name, std::string map, int dist, int time)
		: Name(name), Map(map), Distance(dist), Time(time) {};
};

/*
	"top_y_pos": 720.0,
	"bottom_y_pos": 0.0,
	"item_type": -1,
	"obstacle_type": -1
*/
struct FLineNode
{
	float TopYPos = 0.0f;
	float BottomYPos = 0.0f;
	int ItemType = 0;
	int ObstacleType = 0;

	FLineNode() {};
	FLineNode(float topYPos, float bottomYPos, int itemType, int obstacleType)
		:TopYPos(topYPos), BottomYPos(bottomYPos)
		, ItemType(itemType), ObstacleType(obstacleType) {};
};

/*
{
	"index": 0,
	"name": "Easy",
	"difficulty_color_name": "Cyan",
	"difficulty_rate": 10.0,
	"collision_damage": 20.0,
	"line_node_list": [
		{
			"top_y_pos": 720.0,
			"bottom_y_pos": 0.0,
			"item_type": -1,
			"obstacle_type": -1
		},
*/

namespace EMapInfoText
{
	enum Type
	{
		Name = 0,
		Color,
		DiffRate,
		ColDmg,
		End
	};

	static std::vector<const wchar_t*> gArrMapInfoText =
	{
		TEXT("Name")
		,TEXT("Color")
		,TEXT("DiffRate")
		,TEXT("ColDmg")
	};
}

struct FMapInfo
{
	int Index;
	std::string Name;
	std::string DifficultyColorName;
	float DifficultyRate;
	float CollisionDamage;
	std::vector<FLineNode> lineNodes;

	const wchar_t* GetIconPath(int idx)
	{
		switch (idx)
		{
		case 0:
			return MAP_0_ICON_PATH;
			break;
		case 1:
			return MAP_1_ICON_PATH;
			break;
		case 2:
			return MAP_2_ICON_PATH;
			break;
		}
	}

	const std::wstring GetInfoToWString(EMapInfoText::Type type)
	{
		switch (type)
		{
		case EMapInfoText::Name:
			return std::wstring(Name.begin(), Name.end());
		case EMapInfoText::Color:
			return std::wstring(DifficultyColorName.begin(), DifficultyColorName.end());
		case EMapInfoText::DiffRate:
			return std::to_wstring((int)DifficultyRate);
		case EMapInfoText::ColDmg:
			return std::to_wstring((int)CollisionDamage);
		}
	}
};



/*
{
	"character_list": [
		{
			"index": 0,
			"name": "Yang",
			"color_name": "White",
			"speed": 700.0,
			"hp": 100.0,
			"dex": 100.0,
			"def": 10.0,
			"image_sequence_name": "PlayerIdle",
			"size_x": 200.0,
			"size_y": 135.0
		},
		...
*/

namespace ECharacterStatText
{
	enum Type
	{
		Name = 0,
		HP,
		Speed,
		Dex,
		Def,
		Size,
		End
	};

	static std::vector<const wchar_t*> gArrCharacterStatText =
	{
		TEXT("Name")
		,TEXT("HP")
		,TEXT("Speed")
		,TEXT("Dex")
		,TEXT("Def")
		,TEXT("Size")
	};
}

struct FCharacterState
{
	int Index;
	std::string Name;
	std::string ColorName;
	float Speed;
	float HP;
	float Dex;
	float Def;
	std::string ImageSequenceName;
	float SizeX;
	float SizeY;

	const std::wstring GetStatToWString(ECharacterStatText::Type type)
	{
		switch (type)
		{
		case ECharacterStatText::Name:
			return std::wstring(Name.begin(), Name.end());
		case ECharacterStatText::HP:
			return std::to_wstring((int)HP);
		case ECharacterStatText::Speed:
			return std::to_wstring((int)Speed);
		case ECharacterStatText::Dex:
			return std::to_wstring((int)Dex);
		case ECharacterStatText::Def:
			return std::to_wstring((int)Def);
		case ECharacterStatText::Size:
			return (std::to_wstring((int)SizeX) + L" * " + std::to_wstring((int)SizeY));
		}
	}
};


/*
{
	"db_id": "19e45759635e8028adb0d83e3cf969ff",
	"db_url": "https://api.notion.com/v1/databases/19e45759635e8028adb0d83e3cf969ff",
	"api_key": "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",
	"map_file_list": [
		"map_0.json",
		"map_1.json",
		"map_2.json"
	],
	"character_file": "character_list.json",
	"item_file": "item_list.json",
	"stat_file": "stat_type_list.json"
}
*/
struct FConfig
{
	std::string DatabaseID;
	std::string DatabaseURL;
	std::string APIKey;
	std::vector<std::string> mapFileNameList;
	std::string CharacterFileName;
	std::string ItemFileName;
	std::string StatFileName;
};


/*
{
	"fileName": "sprite_sheet_character",
	"prefix": "attack_down_",
	"sizeX": 600.0,
	"sizeY": 187.0,
	"sprites": [
		{
			"name": "attack_down_0",
			"x": 315.0,
			"y": 144.0,
			"width": 32.0,
			"height": 42.0,
			"pivotX": 0.5,
			"pivotY": 0.0
		},
*/
struct FSpriteSheetInfo
{
	std::string Name;
	float X;
	float Y;
	float Width;
	float Height;
	float PivotX;
	float PivotY;
};

struct FSpriteAtlasInfo
{
	std::string FileName;
	std::string Prefix;
	float SizeX;
	float SizeY;
	std::vector<FSpriteSheetInfo> Sprites;
};


/*
{
	"stat_type_list": [
		{
			"index": 0,
			"type": 0,
			"name": "Hp"
		},
*/
struct FStatInfo
{
	int Index;
	int Type;
	std::string Name;
};


/*
{
	"item_list": [
		{
			"index": 0,
			"name": "Potion",
			"stat_type": 0,
			"add_value": 50.0,
			"desc": "When held, maximum stamina increases."
		},
*/
struct FItemInfo
{
	int Index;
	std::string Name;
	int StatType;
	float AddValue;
	std::string Desc;

	const wchar_t* GetItmeImagePath(int _index)
	{
		switch (_index)
		{
		case 0:
			return ITEM_HP_ICON_PATH;
		case 1:
			return ITEM_SPEED_ICON_PATH;
		case 2:
			return ITEM_DEX_ICON_PATH;
		case 3:
			return ITEM_DEF_ICON_PATH;
		default:
			return L"";
		}
	}
};
