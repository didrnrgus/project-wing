#pragma once
#include "GameInfo.h"

namespace Multiplay 
{
	struct FPlayerInfo
	{
		int id;
		bool isHost;
		std::vector<int> arrItemType;
		int characterType;
		bool isReady;
	};
}

class CMultiplayManager
{
private:
	std::list<Multiplay::FPlayerInfo> mPlayerInfoList;

public:
	void AddPlayer(int _id);
	bool RemovePlayer(int _id);
	Multiplay::FPlayerInfo GetPlayerInfo(int _id);

	DECLARE_SINGLE(CMultiplayManager);
};

