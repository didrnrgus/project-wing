#pragma once
#include "Scene\Scene.h"

class CSceneLobby : public CScene
{
	friend class CSceneManager;

private:
	CSceneLobby();
	virtual ~CSceneLobby();

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();
};

