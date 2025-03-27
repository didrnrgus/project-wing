#include "MultiplayManager.h"

DEFINITION_SINGLE(CMultiplayManager);

CMultiplayManager::CMultiplayManager()
{
	ClearProperties();
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

Multiplay::FPlayerInfo& CMultiplayManager::GetPlayerInfoByID(int _id)
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

const Multiplay::FPlayerInfo CMultiplayManager::GetPlayerInfoByIndex(int _index)
{
	if (_index >= mPlayerInfoList.size()) 
	{
		Multiplay::FPlayerInfo info;
		info.id = -1;
		return info;
	}

	auto it = mPlayerInfoList.begin();
	std::advance(it, _index);
	return *it;
}

void CMultiplayManager::SetHostFromId(int _id)
{
	auto playerInfo = GetPlayerInfoByID(_id);
	playerInfo.isHost = true;
}

void CMultiplayManager::SetPlayerItemFromId(int _senderId, int _slotIndex, int _itemTypeIndex)
{
	auto playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.arrItemType[_slotIndex] = _itemTypeIndex;
}

void CMultiplayManager::SetPlayerCharacterFromId(int _senderId, int _characterIndex)
{
	auto playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.characterType = _characterIndex;
}

void CMultiplayManager::SetPlayerIsReadyFromId(int _senderId, bool _isReady)
{
	auto playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.isReady = _isReady;
}

void CMultiplayManager::SetPlayerIsDeadInGameFromId(int _senderId, bool _isDead)
{
	auto playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.isDeadInGame = _isDead;
}

/*
int main() {
	std::list<int> myList = { 10, 20, 30, 40, 50 };

	auto it = myList.begin();

	std::advance(it, 3);  // 3칸 앞으로 이동 (10 → 40)

	std::cout << "3번째 인덱스 값: " << *it << std::endl;  // 출력: 40

	std::advance(it, -2); // 2칸 뒤로 이동 (40 → 20)

	std::cout << "다시 2칸 뒤로 이동: " << *it << std::endl; // 출력: 20

	return 0;
}
*/
