#pragma once

#include "GameInfo.h"
#include "Etc/JsonContainer.h"

#define BASE_URL "https://api.notion.com/v1/"
#define LAST_UPDATE_DATE "2022-06-28"

class CNotionDBController
{
public:
	/// <summary>
	/// CheckForUpdate() 호출해서 할지말지 따짐.
	/// </summary>
	/// <param name="userInfo"></param>
	/// <returns></returns>
	bool CreateUserRecord(const FUserRankInfo& userInfo);
	bool DeleteRecord(const std::string& page_id);
	bool ReadRecords();

	/// <summary>
	/// 추가 할지말지만 알려주자.
	//	추가 해야할때에는 여기안에서 미리 Delete 해준다.
	/// </summary>
	/// <param name="_userInfo"></param>
	/// <returns></returns>
	bool CheckForUpdate(const FUserRankInfo& _userInfo);

	DECLARE_SINGLE(CNotionDBController);
};
