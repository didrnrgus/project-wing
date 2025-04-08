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

bool CNotionDBController::CreateUserRecord(const FUserRankInfo& userInfo)
{
    CLog::PrintLog("CNotionDBController::CreateUserRecord()");
  
    nlohmann::json jsonData = CJsonController::GetInst()->ConvertToJson(userInfo);
    std::string jsonString = jsonData.dump(4); // JSON을 문자열로 변환
    std::string response = CCURL::GetInst()->SendRequest(NOTION_URL_PAGES
        , METHOD_POST
        , jsonString);

    if (response.empty())
        return false;

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
    sprintf_s(url, "%spages/%s", BASE_URL, page_id);
    nlohmann::json jsonData;
    jsonData[ATT_ARCHIVED] = true;

    std::string response = CCURL::GetInst()->SendRequest(url
    , METHOD_PATCH
    , jsonData.dump());

    if (response.empty())
        return false;

    return true;
}



