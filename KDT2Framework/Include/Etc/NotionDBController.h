#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

#define NOTION_API_KEY "Bearer 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"
#define DATABASE_ID "19e45759635e8028adb0d83e3cf969ff"
#define BASE_URL "https://api.notion.com/v1/"
#define LAST_UPDATE_DATE "2022-06-28"

class CNotionDBController
{
public:
	const std::string GetNotionAPIKey()
	{
		return NOTION_API_KEY;
	}

	const std::string GetLastUpdateDate()
	{
		return LAST_UPDATE_DATE;
	}

	const std::string GetDatabaseID()
	{
		return DATABASE_ID;
	}



	//bool CreateUserRecord(const FUserInfo& userInfo);
	//bool UpdateRecord(const std::string& page_id);
	//bool DeleteRecord(const std::string& page_id);
	//bool ReadRecords(std::map<std::string, FUserInfo>& users);


	template <typename T>
	void PrintRecords(const std::map<std::string, T>& records)
	{
	}

	/*template <>
	void PrintRecords<FUserInfo>(const std::map<std::string, FUserInfo>& users)
	{
		for (const auto& user : users)
		{
			std::cout << "Name: " << user.second.Name <<
				", Phone: " << user.second.PhoneNumber <<
				", Age: " << user.second.Age <<
				", Married: " << (user.second.bIsMarriage ? "Yes" : "No") <<
				", PageID: " << user.first << "\n";
		}
	}*/

	DECLARE_SINGLE(CNotionDBController);
};
