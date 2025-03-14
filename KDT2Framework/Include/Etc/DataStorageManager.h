#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CDataStorageManager
{
private:
	FConfig mConfigData;
	std::map<int, FCharacterState> mCharacterDatas;
	std::map<int, FMapInfo> mMapInfoDatas;

public:
	void SetConfigData(std::string strJson);
	void SetCharacterData(std::string strJson);
	void SetMapData(std::string strJson);

public:
	inline const FConfig GetConfig()
	{
		return mConfigData;
	}

	inline const int GetCharacterCount()
	{
		return mCharacterDatas.size();
	}

	inline const FCharacterState GetCharacterState(int index)
	{
		return mCharacterDatas[index];
	}

	inline const int GetMapInfoCount()
	{
		return mMapInfoDatas.size();
	}

	inline const FMapInfo GetMapInfo(int index)
	{
		return mMapInfoDatas[index];
	}



private:
	DECLARE_SINGLE(CDataStorageManager)
};

