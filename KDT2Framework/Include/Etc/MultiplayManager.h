#pragma once
#include "GameInfo.h"
#include "Etc/ConstValues.h"

namespace Multiplay
{
	struct FPlayerInfo
	{
		int id;
		std::string nickname;
		int characterType;
		bool isHost;
		bool isReady;
		bool isDeadInGame;
		float curHp;
		float distance;
		float height;
		std::vector<int> arrItemType;

		FPlayerInfo()
		{
			id = -1;
			nickname = "";
			isHost = false;
			arrItemType.resize(PLAYER_ITEM_COUNT_MAX, PLAYER_ITEM_TYPE_DEFAULT_INDEX);
			Init();
		}

		void Init()
		{
			characterType = 0;
			isReady = false;
			isDeadInGame = false;
			curHp = 0.0f;
			distance = 0.0f;
			height = 0.0f;

			for (auto& itemType : arrItemType)
			{
				itemType = PLAYER_ITEM_TYPE_DEFAULT_INDEX;
			}
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

	void ResetPlayerAfterInGame()
	{
   		for (auto& playerInfo : mPlayerInfoList)
		{
			playerInfo.Init();
		}
	}

	const Multiplay::FPlayerInfo GetPlayerInfoByIndex(const int _index);
	const Multiplay::FPlayerInfo GetPlayerInfoValueById(const int _id);
	const Multiplay::FPlayerInfo GetPlayerInfoFromMyId();
	int GetPlayerCount() { return mPlayerInfoList.size(); }
	int GetMyId() { return mMyId; }
	int GetIsHost() { return mIsHost; }
	int GetCurMapIndex() { return mCurMapIndex; }
	int GetIsGameStart() { return mIsStart; }

	void ChangeNickname(std::string _newNickname);

private:
	bool IsContainID(int _id);
	void AddPlayer(int _id, std::string _nickname);
	Multiplay::FPlayerInfo& GetPlayerInfoByID(const int _id);
	bool RemovePlayer(const int _id);
	void SetMyId(const int _myId) { mMyId = _myId; }
	void SetCurMapIndex(const int _mapIndex) { mCurMapIndex = _mapIndex; }
	void SetIsGameStart(const bool _isStart) { mIsStart = _isStart; }
	void SetHostFromId(const int _id);
	void SetPlayerItemFromId(const int _senderId, const int _slotIndex, const int _itemTypeIndex);
	void SetPlayerCharacterFromId(const int _senderId, const int _characterIndex);
	void SetPlayerIsReadyFromId(const int _senderId, const bool _isReady);
	void SetPlayerIsDeadInGameFromId(const int _senderId, const bool _isDead);
	void SetPlayerCurHpInGameFromId(const int _senderId, const float _curHp);
	void SetPlayerDistanceInGameFromId(const int _senderId, const float _distance);
	void SetPlayerHeightInGameFromId(const int _senderId, const float _height);
	void SetPlayerNicknameInGameFromId(const int _senderId, const std::string _nickname);
	DECLARE_SINGLE(CMultiplayManager);
};

