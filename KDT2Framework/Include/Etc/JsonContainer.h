#pragma once

#include "GameInfo.h"

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
    "index": 0,
    "name": "Easy",
    "difficulty_rate": 0.1,
    "line_node_list": [
        {
            "top_y_pos": 720.0,
            "bottom_y_pos": 0.0,
            "item_type": -1,
            "obstacle_type": -1
        },
*/
struct FMapInfo
{
	int Index;
	std::string Name;
	std::vector<FLineNode> lineNodes;
};

/*
    "character_list": [
        {
            "index": 0,
            "name": "Yang",
            "color_name": "White",
            "speed": 110.0,
            "hp": 120.0,
            "dex": 130.0,
            "image_sequence_name": "PlayerIdle",
            "size_x": 120.0,
            "size_y": 110.0
        },
        ...
*/
struct FCharacterState
{
	int Index;
    std::string Name;
    std::string ColorName;
    float Speed;
    float HP;
    float Dex;
    std::string ImageSequenceName;
    float SizeX;
    float SizeY;
};

/*{
    "db_id": "19e45759635e8028adb0d83e3cf969ff",
    "db_url": "https://api.notion.com/v1/databases/19e45759635e8028adb0d83e3cf969ff",
    "api_key": "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",
    "map_file_list": [
        "map_0.json",
        "map_1.json",
        "map_2.json"
    ],
    "character_file": "character.json"
}*/
struct FConfig
{
    std::string DatabaseID;
    std::string DatabaseURL;
    std::string APIKey;
    std::vector<std::string> mapFileNameList;
    std::string CharacterFileName;
};

