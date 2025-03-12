#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CDataStorageManager
{
public:
	std::map<int, FMapInfo> mMapStorageDatas;

	DECLARE_SINGLE(CDataStorageManager)
};

