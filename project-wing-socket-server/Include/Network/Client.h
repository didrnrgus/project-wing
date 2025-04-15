#pragma once
#include "GameInfo.h"
#include "Interface/IPlayerStatController.h"

struct CClient : public IPlayerStatController
{
	SOCKET sock;
	int id;
	std::thread thread;
	char nickname[64] = {};

	bool isReady = false;
	bool isAlive = true;
	bool isMovingUp = false;

	int characterId = 0;
	int itemSlots[3] = { -1, -1, -1 };

	float height = 0.0f;
	int lastObstacleStep = 0;  // 16m 단위로 체크됨

	void Init()
	{
		isReady = false;
		isAlive = true;
		isMovingUp = false;

		characterId = 0;
		itemSlots[0] = -1;
		itemSlots[1] = -1;
		itemSlots[2] = -1;

		height = 0.0f;
		lastObstacleStep = 0;

		std::cout << "client_" << id
			<< " Init(): " << "\n";
	}
};