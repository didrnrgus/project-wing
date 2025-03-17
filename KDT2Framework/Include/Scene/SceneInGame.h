#pragma once
#include "Scene\Scene.h"
#include "Interface/IPlayerInGameController.h"

class CSceneInGame : public CScene, public IPlayerInGameController
{
	friend class CSceneManager;

private:
	CSceneInGame();
	virtual ~CSceneInGame();

protected:
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;

public:
	virtual CSceneObject* GetPlayer(int index) override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
	virtual bool SetMovePlayer(int playerIndex, FVector3D moveValVector) override;
};


