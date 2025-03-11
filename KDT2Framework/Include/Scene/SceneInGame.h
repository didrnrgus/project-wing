#pragma once
#include "Scene\Scene.h"
#include "Interface/IPlayerController.h"

class CSceneInGame : public CScene, public IPlayerController
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
	virtual int GetGraphicCount() override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
};


