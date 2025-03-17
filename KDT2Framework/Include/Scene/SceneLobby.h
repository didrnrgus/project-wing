#pragma once
#include "Scene\Scene.h"
#include "Interface/IPlayerGraphicController.h"

class CSceneLobby : public CScene, public IPlayerGraphicController
{
	friend class CSceneManager;

private:
	CSceneLobby();
	virtual ~CSceneLobby();

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();

public:
	virtual CSceneObject* GetPlayer(int index) override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
};

