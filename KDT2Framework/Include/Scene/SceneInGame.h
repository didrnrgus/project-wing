#pragma once
#include "Scene\Scene.h"
#include "Interface/IPlayerInGameController.h"
#include "Interface/IGamePlayController.h"

class CSceneInGame : public CScene, public IPlayerInGameController
{
	friend class CSceneManager;

private:
	CSceneInGame();
	virtual ~CSceneInGame();

private:
	std::vector<IGamePlayController*> mArrGamePlayCtlr;

protected:
	virtual bool Init() override;
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;
	virtual CSceneObject* GetPlayer(int index) override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
	virtual bool SetMovePlayer(int playerIndex, FVector3D moveValVector) override;
	
	void SetGamePlayState(EGamePlayState::Type type);
};


