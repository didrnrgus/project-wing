#include "MultiplayManager.h"
#include "Etc/NetworkManager.h"

DEFINITION_SINGLE(CMultiplayManager);

CMultiplayManager::CMultiplayManager()
{
	ClearProperties();
}

CMultiplayManager::~CMultiplayManager()
{

}

void CMultiplayManager::ChangeNickname(std::string _newNickname)
{
	//auto& info = GetPlayerInfoByID(mMyId);
	//info.nickname = _newNickname;

	std::vector<char> buffer(_newNickname.begin(), _newNickname.end());
	CNetworkManager::GetInst()->SendMsg(mMyId, (int)ClientMessage::MSG_CHANGE_NICKNAME, buffer.data(), buffer.size());
}

bool CMultiplayManager::IsContainID(int _id)
{
	return std::any_of(mPlayerInfoList.begin(), mPlayerInfoList.end(),
		[_id](const Multiplay::FPlayerInfo& info)
		{
			return info.id == _id;
		});
}

void CMultiplayManager::AddPlayer(const int _id, std::string _nickname)
{
	Multiplay::FPlayerInfo info;
	info.id = _id;
	info.nickname = _nickname;
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

const Multiplay::FPlayerInfo CMultiplayManager::GetPlayerInfoValueById(const int _id)
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

const Multiplay::FPlayerInfo CMultiplayManager::GetPlayerInfoFromMyId()
{
	auto it = std::find_if(mPlayerInfoList.begin(), mPlayerInfoList.end(),
		[this](const Multiplay::FPlayerInfo _info)
		{
			return _info.id == mMyId;
		});

	if (it != mPlayerInfoList.end())
	{
		return *it;
	}

	Multiplay::FPlayerInfo info;
	info.id = -1;
	return info;
}

void CMultiplayManager::SetHostFromId(const int _id)
{
	if (!IsContainID(_id))
		return;

	auto& playerInfo = GetPlayerInfoByID(_id);
	playerInfo.isHost = true;

	if (_id == GetMyId())
	{
		mIsHost = true;
	}
}

void CMultiplayManager::SetPlayerItemFromId(const int _senderId, const int _slotIndex, const int _itemTypeIndex)
{
	if(!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.arrItemType[_slotIndex] = _itemTypeIndex;
}

void CMultiplayManager::SetPlayerCharacterFromId(const int _senderId, const int _characterIndex)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.characterType = _characterIndex;
}

void CMultiplayManager::SetPlayerIsReadyFromId(const int _senderId, const bool _isReady)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.isReady = _isReady;
}

void CMultiplayManager::SetPlayerIsDeadInGameFromId(const int _senderId, const bool _isDead)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.isDeadInGame = _isDead;
}

void CMultiplayManager::SetPlayerCurHpInGameFromId(const int _senderId, const float _curHp)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.curHp = _curHp;
}

void CMultiplayManager::SetPlayerDistanceInGameFromId(const int _senderId, const float _distance)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.distance = _distance;
}

void CMultiplayManager::SetPlayerHeightInGameFromId(const int _senderId, const float _height)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.height = _height;
}

void CMultiplayManager::SetPlayerNicknameInGameFromId(const int _senderId, const std::string _nickname)
{
	if (!IsContainID(_senderId))
		return;
	auto& playerInfo = GetPlayerInfoByID(_senderId);
	playerInfo.nickname = _nickname;
}

