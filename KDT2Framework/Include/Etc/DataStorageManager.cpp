#include "DataStorageManager.h"
#include "Etc/JsonController.h"

DEFINITION_SINGLE(CDataStorageManager);

CDataStorageManager::CDataStorageManager()
{

}

CDataStorageManager::~CDataStorageManager()
{

}

void CDataStorageManager::SetConfigData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mConfigData);
}

void CDataStorageManager::SetCharacterData(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mCharacterDatas);
}

void CDataStorageManager::SetMapData(std::string strJson)
{
	// 한개 스테이지의 맵.
	FMapInfo info;
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, info);
	mMapInfoDatas.insert(std::make_pair(info.Index, info));
}

void CDataStorageManager::SetSpriteAtlasInfo(std::string strJson)
{
	nlohmann::json json = nlohmann::json::parse(strJson);
	CJsonController::GetInst()->ParseJson(json, mSpriteAtlasInfoBySpriteName);
	CJsonController::GetInst()->ParseJson(json, mSpriteAtlasInfoByFileName);
}

const FLineNode CDataStorageManager::GetLineNodeInSelectedMap(int lineNodeIndex)
{
	int lineCount = GetLineNodeCountInSelectedMap();
	int useIndex = lineNodeIndex % lineCount;
	return mMapInfoDatas[curSelectedMapIndex].lineNodes[useIndex];
}


