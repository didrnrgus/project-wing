#include "MultiplayManager.h"

DEFINITION_SINGLE(CMultiplayManager);

CMultiplayManager::CMultiplayManager()
{

}

CMultiplayManager::~CMultiplayManager()
{

}

void CMultiplayManager::AddPlayer(int _id)
{
	Multiplay::FPlayerInfo info;
	info.id = _id;
	mPlayerInfoList.push_back(info);
}

bool CMultiplayManager::RemovePlayer(int _id)
{
	auto it = std::find_if(mPlayerInfoList.begin(), mPlayerInfoList.end(),
		[_id](const Multiplay::FPlayerInfo _info)
		{
			return _info.id == _id;
		});

	if (it != mPlayerInfoList.end())
	{
		mPlayerInfoList.erase(it);
		return true;
	}

	return false;
}

Multiplay::FPlayerInfo CMultiplayManager::GetPlayerInfo(int _id)
{
	auto it = std::find_if(mPlayerInfoList.begin(), mPlayerInfoList.end(),
		[_id](const Multiplay::FPlayerInfo _info)
		{
			return _info.id == _id;
		});

	if (it != mPlayerInfoList.end())
	{
		return *it;
	}

	Multiplay::FPlayerInfo info;
	info.id = -1;
	return info;
}
