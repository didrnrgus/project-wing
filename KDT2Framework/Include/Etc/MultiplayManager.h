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
	friend class CNetworkManager;

private:
	std::list<Multiplay::FPlayerInfo> mPlayerInfoList;
	int mMyId;
	bool mIsHost;
	int mCurMapIndex;
	bool mIsStart;

public:
	void ClearProperties() 
	{
		mPlayerInfoList.clear();
		mMyId = -1;
		mCurMapIndex = 0;
		mIsStart = false;
		mIsHost = false;
	}

	// 리턴: 값복사
	const Multiplay::FPlayerInfo GetPlayerInfoByIndex(int _index);
	const Multiplay::FPlayerInfo GetPlayerInfoByMyId() { return GetPlayerInfoByID(mMyId); }
	int GetPlayerCount() { return mPlayerInfoList.size(); }
	int GetMyId() { return mMyId; }
	int GetIsHost() { return mIsHost; }
	int GetCurMapIndex() { return mCurMapIndex; }
	int GetIsGameStart() { return mIsStart; }

private:
	void AddPlayer(int _id);
	// 리턴: 레퍼런스
	Multiplay::FPlayerInfo& GetPlayerInfoByID(int _id);
	bool RemovePlayer(int _id);
	void SetMyId(int _myId) { mMyId = _myId; }
	void SetCurMapIndex(int _mapIndex) { mCurMapIndex = _mapIndex; }
	void SetIsGameStart(bool _isStart) { mIsStart = _isStart; }
	void SetHostFromId(int _id);
	void SetPlayerItemFromId(int _senderId, int _slotIndex, int _itemTypeIndex);
	void SetPlayerCharacterFromId(int _senderId, int _characterIndex);
	void SetPlayerIsReadyFromId(int _senderId, bool _isReady);
	void SetPlayerIsDeadInGameFromId(int _senderId, bool _isDead);

	DECLARE_SINGLE(CMultiplayManager);
};

