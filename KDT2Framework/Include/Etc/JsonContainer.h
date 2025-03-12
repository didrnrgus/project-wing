#pragma once

#include "GameInfo.h"

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

struct FMapInfo
{
	int Index;
	std::string Name;
	std::vector<FLineNode> lineNodes;
};