#pragma once
#include "Scene\Scene.h"
#include "Interface/IScenePlayerInGameController.h"
#include "Interface/IGamePlayStateController.h"

class CSceneInGame : public CScene, public IScenePlayerInGameController
{
	friend class CSceneManager;

private:
	CSceneInGame();
	virtual ~CSceneInGame();

private:
	std::vector<IGamePlayStateController*> mArrGamePlayCtlr;
	EGamePlayState::Type mGamePlayState = EGamePlayState::Ready;

protected:
	virtual bool Init() override;
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;
	virtual CSceneObject* GetPlayer(int index) override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
	virtual bool SetMovePlayer(int playerIndex, FVector3D moveValVector) override;
	
	void SetGamePlayState(EGamePlayState::Type type);
	EGamePlayState::Type GetGamePlayState() { return mGamePlayState; }
};


