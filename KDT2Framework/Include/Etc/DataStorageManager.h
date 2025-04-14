#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

class CDataStorageManager
{
private:
	FConfig mConfigData;
	std::map<int, FCharacterState> mCharacterInfoDatas;
	std::map<std::string, FStatInfo> mStatInfoDatasByName;
	std::map<std::string, FColorInfo> mColorInfoDatasByName;
	std::map<int, FItemInfo> mItemInfoDatasByIndex;
	std::map<int, FMapInfo> mMapInfoDatasByIndex;

	// 아틀라스 내 slace 한스프라이트 시트요소이름.
	std::map<std::string, std::map<std::string, FSpriteSheetInfo>> mSpriteAtlasInfoBySpriteName;
	std::map<std::string, FSpriteAtlasInfo> mSpriteAtlasInfoByFileName;

	std::map<std::string, FUserRankInfo> mUserRankInfosByPageId;

private:
	int curSelectedMapIndex;
	int curSelectedCharacterIndex;
	std::map<int, int> curSelectedItemIndex;
	bool isLoadedData = false;
	FUserRankInfo curUserResult;
	bool isNewRecord = false;

public:
	void InitCurSelectedData();
	void SetConfigData(std::string strJson);
	void SetCharacterData(std::string strJson);
	void SetStatInfoData(std::string strJson);
	void SetColorInfoData(std::string strJson);
	void SetItemInfoData(std::string strJson);
	void SetMapData(std::string strJson);
	void SetSpriteAtlasInfo(std::string strJson);
	void SetRankData(std::string strJson);

	inline void SetIsLoadedData(bool _isLoaded) { isLoadedData = _isLoaded; }
	inline void SetSelectedMapIndex(int mapIndex) { curSelectedMapIndex = mapIndex; }
	inline void SetSelectedCharacterIndex(int characterIndex) { curSelectedCharacterIndex = characterIndex; }
	inline void SetSelectedItemTypeInSlotIndex(int slotIndex, int itemTypeIndex) { curSelectedItemIndex[slotIndex] = itemTypeIndex; }

	void ClearCurUserResult() { curUserResult = FUserRankInfo(); }
	void SetCurUserResult(FUserRankInfo _info, bool _isNewRecord)
	{
		curUserResult = _info;
		isNewRecord = _isNewRecord;
	}
	FUserRankInfo GetCurUserResult() { return curUserResult; }
	bool GetIsNewRecord() { return isNewRecord; }
	void UpdateUserRankInfos();

	inline bool GetIsLoadedData() { return isLoadedData; }
	inline const FConfig GetConfig() { return mConfigData; }
	inline const int GetCharacterCount() { return mCharacterInfoDatas.size(); }
	inline const int GetSelectableItemCount() { return mConfigData.SelectableItemCount; }

	//인덱스에 따른 캐릭터 가져오기.
	inline const FCharacterState GetCharacterState(int index) { return mCharacterInfoDatas[index]; }

	// 내가 고른 캐릭의 데이터
	inline const FCharacterState GetSelectedCharacterState() { return mCharacterInfoDatas[curSelectedCharacterIndex]; }

	// 이건 캐릭 종류의 인덱스
	inline const int GetSelectedCharacterIndex() { return curSelectedCharacterIndex; }

	inline const int GetMapInfoCount() { return mMapInfoDatasByIndex.size(); }
	inline const FMapInfo GetMapInfo(int index) { return mMapInfoDatasByIndex[index]; }
	inline const FMapInfo GetSelectedMapInfo() { return mMapInfoDatasByIndex[curSelectedMapIndex]; }
	inline const int GetSelectedMapIndex() { return curSelectedMapIndex; }
	inline const int GetLineNodeCountInSelectedMap() { return mMapInfoDatasByIndex[curSelectedMapIndex].lineNodes.size(); }
	const FLineNode GetLineNodeInSelectedMap(int lineNodeIndex);

	inline const FVector2D GetSpritSheetImageSize(std::string keyFileName)
	{
		auto info = mSpriteAtlasInfoByFileName[keyFileName];
		return FVector2D(info.SizeX, info.SizeY);
	}

	inline const int GetSpritSheetCount(std::string keyFileName)
	{
		return mSpriteAtlasInfoBySpriteName[keyFileName].size();
	}

	inline std::vector<FSpriteSheetInfo> GetFilteredSpriteSheets(const std::string& keyFileName, const std::string& filterStr)
	{
		std::vector<FSpriteSheetInfo> filteredSheets;

		auto it = mSpriteAtlasInfoBySpriteName.find(keyFileName);
		if (it != mSpriteAtlasInfoBySpriteName.end()) // keyFileName 존재 확인
		{
			for (const auto& pair : it->second)
			{
				const FSpriteSheetInfo& sheetInfo = pair.second;
				if (sheetInfo.Name.find(filterStr) != std::string::npos) // 특정 문자열 포함 여부 확인
				{
					filteredSheets.push_back(sheetInfo);
				}
			}
		}
		return filteredSheets;
	}

	inline const std::string GetSpritSheetPrefix(std::string keyFileName)
	{
		return mSpriteAtlasInfoByFileName[keyFileName].Prefix;
	}

	inline const FSpriteSheetInfo GetSpritSheetInfo(std::string keyFileName, std::string keySpriteName)
	{
		return mSpriteAtlasInfoBySpriteName[keyFileName][keySpriteName];
	}

	inline const std::map<int, FItemInfo> GetItemInfoDatas() { return mItemInfoDatasByIndex; }
	inline const FItemInfo GetItemInfoDataByIndex(int index) { return mItemInfoDatasByIndex[index]; }
	inline const int GetCurSelectedItemIDBySlotIndex(int index) { return curSelectedItemIndex[index]; }
	std::vector<FUserRankInfo> GetArrayUserRankByCategory(ERankMenuTap::Type _mainCategoryType, int _subCategoryValue);
	inline const FColorInfo GetColorInfoFromName(std::string _colorName)
	{
		return mColorInfoDatasByName[_colorName];
	}
private:
	DECLARE_SINGLE(CDataStorageManager)
};

