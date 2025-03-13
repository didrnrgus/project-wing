#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CDataStorageManager
{
private:
	FConfig mConfigData;
	std::map<int, FMapInfo> mMapStorageDatas;

public:
	void SetConfigData(std::string);

private:
	DECLARE_SINGLE(CDataStorageManager)
};

