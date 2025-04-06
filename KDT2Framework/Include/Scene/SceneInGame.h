#pragma once
#include "Scene\Scene.h"
#include "Interface/IScenePlayerGraphicController.h"
#include "Interface/ISceneNetworkController.h"

class CSceneInGame : public CScene, public IScenePlayerGraphicController, public ISceneNetworkController
{
	friend class CSceneManager;

private:
	CSceneInGame();
	virtual ~CSceneInGame();

private:
	class CPlayerInGameObject* mMyPlayerObject = nullptr;
	std::vector<class IGamePlayStateController*> mArrGamePlayStateCtlr;
	EGamePlayState::Type mGamePlayState = EGamePlayState::Ready;
	int mCurReadyCount;
	float mReadyCountTime;
	class CInGameWidget* mInGameWidget = nullptr;
	
	int mTaskID;

protected:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;

public:
	// 플레이어 캐릭터 종류가 아닌 현재방의 사용자캐릭을 불러오는것이다.
	virtual CSceneObject* GetPlayer(int index) override;
	bool GetOtherPlayerStatByNetId(int _netId, class IPlayerStatController*& _playerStatOut);

	// 플레이어/서버/씬도 사용할 수 있음.
	void SetGamePlayState(EGamePlayState::Type type);
	EGamePlayState::Type GetGamePlayState() { return mGamePlayState; }
	class CPlayerInGameObject* GetMyPlayerObject() { return mMyPlayerObject; }

	// ISceneNetworkController을(를) 통해 상속됨
	void ProcessMessage() override;
	void DistributeMessage(const RecvMessage& msg) override;
	void AddListener(IObjectNetworkController* obj) override;
	void RemoveListener(IObjectNetworkController* obj) override;
};


