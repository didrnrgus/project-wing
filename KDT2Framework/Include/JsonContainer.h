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

