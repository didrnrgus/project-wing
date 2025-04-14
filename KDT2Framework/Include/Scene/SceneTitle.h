#pragma once
#include "Scene/Scene.h"
#include "Interface/ISceneNetworkController.h"
#include "Interface/IScenePlayerGraphicController.h"

class CSceneTitle : public CScene, public ISceneNetworkController, public IScenePlayerGraphicController
{
	friend class CSceneManager;

private:
	CSceneTitle();
	virtual ~CSceneTitle();

private:
	class CTitleWidget* mTitleWidget;
	class CPlayerTitleObject* mMyPlayerObject = nullptr;
	std::vector<class IGamePlayStateController*> mArrGamePlayStateCtlr;
	EGamePlayState::Type mGamePlayState = EGamePlayState::Start;

protected:
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;
	virtual void Update(float DeltaTime) override;

	// ISceneNetworkController을(를) 통해 상속됨
	virtual void ProcessMessage() override;
	virtual void DistributeMessage(const struct RecvMessage& msg) override;

public:
	// ISceneNetworkController을(를) 통해 상속됨
	virtual void AddListener(class IObjectNetworkController* obj) override;
	virtual void RemoveListener(class IObjectNetworkController* obj) override;

	void SetGamePlayState(EGamePlayState::Type type);
	EGamePlayState::Type GetGamePlayState() { return mGamePlayState; }

	// IScenePlayerGraphicController을(를) 통해 상속됨
	CSceneObject* GetPlayer(int index) override;
	bool SetChangeGraphic(int playerIndex, int graphicIndex) override;

	void InitObjectAfterDataLoad();
};

