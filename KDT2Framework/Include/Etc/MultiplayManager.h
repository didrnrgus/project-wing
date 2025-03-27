#pragma once
#include "GameInfo.h"
#include "Etc/ConstValues.h"

namespace Multiplay
{
	struct FPlayerInfo
	{
		int id;
		int characterType;
		bool isHost;
		bool isReady;
		bool isDeadInGame;
		std::vector<int> arrItemType;

		FPlayerInfo()
		{
			id = -1;
			characterType = 0;
			isHost = false;
			isReady = false;
			isDeadInGame = false;
			arrItemType.resize(PLAYER_ITEM_COUNT_MAX, PLAYER_ITEM_TYPE_DEFAULT_INDEX);
		}
	};
}

class CMultiplayManager
{
private:
	std::list<Multiplay::FPlayerInfo> mPlayerInfoList;
	int mCurMapIndex;
	bool mIsStart;

public:
	void ClearProperties() 
	{
		mPlayerInfoList.clear();
		mCurMapIndex = 0;
		mIsStart = false;
	}

	void AddPlayer(int _id);
	bool RemovePlayer(int _id);
	int GetPlayerCount() { return mPlayerInfoList.size(); }
	// 리턴 값 읽기용도.
	const Multiplay::FPlayerInfo GetPlayerInfoByIndex(int _index);

	void SetCurMapIndex(int _mapIndex) { mCurMapIndex = _mapIndex; }
	int GetCurMapIndex() { return mCurMapIndex; }
	void SetIsGameStart(bool _isStart) { mIsStart = _isStart; }

	void SetHostFromId(int _id);
	void SetPlayerItemFromId(int _senderId, int _slotIndex, int _itemTypeIndex);
	void SetPlayerCharacterFromId(int _senderId, int _characterIndex);
	void SetPlayerIsReadyFromId(int _senderId, bool _isReady);
	void SetPlayerIsDeadInGameFromId(int _senderId, bool _isDead);
private:
	// 리턴 값 내부 변경 가능.
	Multiplay::FPlayerInfo& GetPlayerInfoByID(int _id);

	DECLARE_SINGLE(CMultiplayManager);
};

