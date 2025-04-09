#include "DataStorageManager.h"
#include "Etc/JsonController.h"
#include "Etc/NotionDBController.h"

DEFINITION_SINGLE(CDataStorageManager);

CDataStorageManager::CDataStorageManager()
{

}

CDataStorageManager::~CDataStorageManager()
{

}

void CDataStorageManager::InitCurSelectedData()
{
	curSelectedCharacterIndex = 0;
	curSelectedMapIndex = 0;

	for (auto& e : curSelectedItemIndex)
	{
		e.second = -1;
	}
}

void CDataStorageManager::SetConfigData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mConfigData);

	for (int i = 0; i < mConfigData.SelectableItemCount; i++)
	{
		curSelectedItemIndex.insert(std::make_pair(i, -1));
	}
}

void CDataStorageManager::SetCharacterData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mCharacterInfoDatas);
}

void CDataStorageManager::SetStatInfoData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mStatInfoDatasByName);
}

void CDataStorageManager::SetItemInfoData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mItemInfoDatasByIndex);
}

void CDataStorageManager::SetMapData(std::string strJson)
{
	// 한개 스테이지의 맵.
	FMapInfo info;
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, info);
	mMapInfoDatasByIndex.insert(std::make_pair(info.Index, info));
}

void CDataStorageManager::SetSpriteAtlasInfo(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mSpriteAtlasInfoBySpriteName);
	CJsonController::GetInst()->ParseJson(json, mSpriteAtlasInfoByFileName);
}

void CDataStorageManager::SetRankData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mUserRankInfosByPageId);
}

void CDataStorageManager::UpdateUserRankInfos()
{
	mUserRankInfosByPageId.clear();
	CNotionDBController::GetInst()->ReadRecords();
}

const FLineNode CDataStorageManager::GetLineNodeInSelectedMap(int lineNodeIndex)
{
	int lineCount = GetLineNodeCountInSelectedMap();
	int useIndex = lineNodeIndex % lineCount;
	return mMapInfoDatasByIndex[curSelectedMapIndex].lineNodes[useIndex];
}

std::vector<FUserRankInfo> CDataStorageManager::GetArrayUserRankByCategory(EResultMenuTap::Type _type, int _value)
{
	std::vector<FUserRankInfo> result;

	for (const auto& pair : mUserRankInfosByPageId)
	{
		const FUserRankInfo& info = pair.second;

		if (_type == EResultMenuTap::Map)
		{
			if (info.Map == _value)
			{
				result.push_back(info);
			}
		}
		else if (_type == EResultMenuTap::Character)
		{
			if (info.Character == _value)
			{
				result.push_back(info);
			}
		}
		
	}

	std::sort(result.begin(), result.end(), [](const FUserRankInfo& a, const FUserRankInfo& b) {
		return a.Distance > b.Distance; // 내림차순
		});

	return result;
}


