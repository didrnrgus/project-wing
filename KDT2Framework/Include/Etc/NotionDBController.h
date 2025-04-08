#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

#define BASE_URL "https://api.notion.com/v1/"
#define LAST_UPDATE_DATE "2022-06-28"

class CNotionDBController
{
public:
	bool CreateUserRecord(const FUserRankInfo& userInfo);
	//bool UpdateRecord(const std::string& page_id);
	bool DeleteRecord(const std::string& page_id);
	bool ReadRecords();

	DECLARE_SINGLE(CNotionDBController);
};
