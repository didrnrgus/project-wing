#pragma once

#include "GameInfo.h"

struct FLineNode
{
	int ID;
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
	std::string Name;
	std::string Map;
	int Age;
	bool bIsMarriage;

	FUserInfo() {};
};

struct FMapInfo
{
	int ID;
	std::string Name;
	std::vector<FLineNode> lineNodes;
};