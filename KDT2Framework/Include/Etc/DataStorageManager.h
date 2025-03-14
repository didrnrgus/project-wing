#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CDataStorageManager
{
private:
	FConfig mConfigData;
	std::map<int, FCharacterState> mCharacterDatas;
	std::map<int, FMapInfo> mMapStorageDatas;

public:
	void SetConfigData(std::string strJson);
	void SetCharacterData(std::string strJson);
	void SetMapData(std::string strJson);

public:
	inline const FConfig GetConfig()
	{
		return mConfigData;
	}

private:
	DECLARE_SINGLE(CDataStorageManager)
};

