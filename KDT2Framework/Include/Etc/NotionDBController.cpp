#pragma once

#include "Etc/NotionDBController.h"
#include "Etc/CURL.h"
#include "Etc/JsonController.h"
#include "Etc/DataStorageManager.h"

DEFINITION_SINGLE(CNotionDBController);

CNotionDBController::CNotionDBController()
{

}

CNotionDBController::~CNotionDBController()
{

}

bool CNotionDBController::CreateUserRecord(const FUserRankInfo& _userInfo, std::string& _newRankPageIdOut)
{
    CLog::PrintLog("CNotionDBController::CreateUserRecord() Check Score By Category.");
  
    if (!CheckForUpdate(_userInfo))
    {
        CLog::PrintLog("CNotionDBController::CreateUserRecord() Condition not met.");
        return false;
    }

    CLog::PrintLog("CNotionDBController::CreateUserRecord() Insert Row into Database.");
    nlohmann::json jsonData = CJsonController::GetInst()->ConvertToJson(_userInfo);

#ifdef _DEBUG
    std::string jsonString = jsonData.dump(4); // JSON을 문자열로 변환
#else
    std::string jsonString = jsonData.dump();
#endif // _DEBUG

    std::string response = CCURL::GetInst()->SendRequest(NOTION_URL_PAGES
        , METHOD_POST
        , jsonString);

    if (response.empty())
        return false;

    nlohmann::json json = nlohmann::json::parse(response);
    FUserRankInfo _info = CJsonController::GetInst()->ParseJsonFUserRankInfo(json);
    _newRankPageIdOut = _info.PageId;
    CDataStorageManager::GetInst()->UpdateUserRankInfos();

    return true;
}

bool CNotionDBController::ReadRecords()
{
    CLog::PrintLog("CNotionDBController::ReadRecords()");

    char url[128] = {};
    sprintf_s(url, NOTION_URL_FORMAT_DATABSE_ID_QUERY, CDataStorageManager::GetInst()->GetConfig().DatabaseID.c_str());

    // Notion API는 GET이 아니라 POST 사용!
    std::string response = CCURL::GetInst()->SendRequest(url
        , METHOD_POST
        , "{}"); 

    if (response.empty())
        return false;
    
    CDataStorageManager::GetInst()->SetRankData(response);

    return true;
}

bool CNotionDBController::CheckForUpdate(const FUserRankInfo& _userInfo)
{
     // 먼저 넣기전에 현재 있는 데이터에서 데이터 현황 확인 -> 5개 이상? 4등까지 남기고 삭제.
    auto _mapResult = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(
        EResultMenuTap::Map, _userInfo.Map);
    auto _characterResult = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(
        EResultMenuTap::Character, _userInfo.Character);

    // 별일 없으면 그냥 추가.
    bool _isMapUpdate = true;
    bool _isCharacterUpdate = true;

    auto _config = CDataStorageManager::GetInst()->GetConfig();
    if (_mapResult.size() >= _config.RankCountByCategory)
    {
        _isMapUpdate = false;

        // 라스트 받고, 비교해서 Insert 할지 말지.
        auto _last = _mapResult[_mapResult.size() - 1];
        bool _needDelete = _last.Distance < _userInfo.Distance;
        _isMapUpdate = _needDelete;

        if (_needDelete)
            DeleteRecord(_last.PageId);
    }

    if (_characterResult.size() >= _config.RankCountByCategory)
    {
        _isCharacterUpdate = false;
        
        // 라스트 받고, 비교해서 Insert 할지 말지.
        auto _last = _characterResult[_characterResult.size() - 1];
        bool _needDelete = _last.Distance < _userInfo.Distance;
        _isCharacterUpdate = _needDelete;

        if (_needDelete)
            DeleteRecord(_last.PageId);
    }

    return _isMapUpdate || _isCharacterUpdate;
}

//bool CNotionDBController::UpdateRecord(const std::string& page_id)
//{
//    const std::string url = BASE_URL + "pages/" + page_id;
//    nlohmann::json requestBody;
//    requestBody[PROPERTIES][PROP_PHONE_NUMBER][PROP_PHONE_NUMBER] = "010-1234-5678";
//
//    // 변경한 해당 레코드를 반환해준다.
//    std::string response = CCURL::GetInst()->SendRequest(url
//        , METHOD_PATCH
//        , requestBody.dump());
//
//    if (response.empty())
//        return false;
//    
//    return true;
//}

bool CNotionDBController::DeleteRecord(const std::string& page_id)
{
    char url[128] = {};
    sprintf_s(url, "%spages/%s", BASE_URL, page_id.c_str());
    nlohmann::json jsonData;
    jsonData[ATT_ARCHIVED] = true;

    std::string response = CCURL::GetInst()->SendRequest(url
    , METHOD_PATCH
    , jsonData.dump());

    if (response.empty())
        return false;

    return true;
}



