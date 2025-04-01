#include "MultiplayManager.h"

DEFINITION_SINGLE(CMultiplayManager);

CMultiplayManager::CMultiplayManager()
{
	ClearProperties();
}

CMultiplayManager::~CMultiplayManager()
{

}

void CMultiplayManager::AddPlayer(const int _id)
{
	Multiplay::FPlayerInfo info;
	info.id = _id;
	mPlayerInfoList.push_back(info);
}

bool CMultiplayManager::RemovePlayer(const int _id)
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

Multiplay::FPlayerInfo& CMultiplayManager::GetPlayerInfoByID(const int _id)
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

const Multiplay::FPlayerInfo CMultiplayManager::GetPlayerInfoByIndex(const int _index)
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

	/*int main() {
			std::list<int> myList = { 10, 20, 30, 40, 50 };
			auto it = myList.begin();
			std::advance(it, 3);  // 3칸 앞으로 이동 (10 → 40)
			std::cout << "3번째 인덱스 값: " << *it << std::endl;  // 출력: 40
			std::advance(it, -2); // 2칸 뒤로 이동 (40 → 20)
			std::cout << "다시 2칸 뒤로 이동: " << *it << std::endl; // 출력: 20
			return 0;
		}*/

}

void CMultiplayManager::SetHostFromId(const int _id)
{
	auto& playerInfo = GetPlayerInfoByID(_id);
	playerInfo.isHost = true;

	if (_id == GetMyId())
	{
		mIsHost = true;
	}
}

void CMultiplayManager::SetPlayerItemFromId(const int _senderId, const int _slotIndex, const int _itemTypeIndex)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.arrItemType[_slotIndex] = _itemTypeIndex;
}

void CMultiplayManager::SetPlayerCharacterFromId(const int _senderId, const int _characterIndex)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.characterType = _characterIndex;
}

void CMultiplayManager::SetPlayerIsReadyFromId(const int _senderId, const bool _isReady)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.isReady = _isReady;
}

void CMultiplayManager::SetPlayerIsDeadInGameFromId(const int _senderId, const bool _isDead)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.isDeadInGame = _isDead;
}

void CMultiplayManager::SetPlayerCurHpInGameFromId(const int _senderId, const float _curHp)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.curHp = _curHp;
}

void CMultiplayManager::SetPlayerDistanceInGameFromId(const int _senderId, const float _distance)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.distance = _distance;
}

void CMultiplayManager::SetPlayerHeightInGameFromId(const int _senderId, const float _height)
{
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.height = _height;
}

