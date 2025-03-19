#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CDataStorageManager
{
private:
	FConfig mConfigData;
	std::map<int, FCharacterState> mCharacterDatas;
	std::map<int, FMapInfo> mMapInfoDatas;

private:
	int curSelectedMapIndex;
	int curSelectedCharacterIndex;

public:
	void SetConfigData(std::string strJson);
	void SetCharacterData(std::string strJson);
	void SetMapData(std::string strJson);

	inline void SetSelectedMapIndex(int mapIndex) { curSelectedMapIndex = mapIndex; }
	inline void SetSelectedCharacterIndex(int characterIndex) { curSelectedCharacterIndex = characterIndex; }

	inline const FConfig GetConfig() { return mConfigData; }
	inline const int GetCharacterCount() { return mCharacterDatas.size(); }

	//인덱스에 따른 캐릭터 가져오기.
	inline const FCharacterState GetCharacterState(int index) { return mCharacterDatas[index]; }

	// 내가 고른 캐릭의 데이터
	inline const FCharacterState GetSelectedCharacterState() { return mCharacterDatas[curSelectedCharacterIndex]; }

	// 이건 캐릭 종류의 인덱스
	inline const int GetSelectedCharacterIndex() { return curSelectedCharacterIndex; }

	inline const int GetMapInfoCount() { return mMapInfoDatas.size(); }
	inline const FMapInfo GetMapInfo(int index) { return mMapInfoDatas[index]; }
	inline const FMapInfo GetSelectedMapInfo() { return mMapInfoDatas[curSelectedMapIndex]; }
	inline const int GetSelectedMapIndex() { return curSelectedMapIndex; }
	inline const int GetLineNodeCountInSelectedMap() { return mMapInfoDatas[curSelectedMapIndex].lineNodes.size(); }
	const FLineNode GetLineNodeInSelectedMap(int lineNodeIndex);

private:
	DECLARE_SINGLE(CDataStorageManager)
};

