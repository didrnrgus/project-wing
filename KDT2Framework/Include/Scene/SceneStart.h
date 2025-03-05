#pragma once

#include "Scene/Scene.h"

class CSceneStart :
    public CScene
{
	friend class CSceneManager;

private:
	CSceneStart();
	virtual ~CSceneStart();

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();
};

