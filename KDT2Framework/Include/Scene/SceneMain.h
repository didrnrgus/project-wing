#pragma once

#include "Scene/Scene.h"

class CSceneMain :
    public CScene
{
	friend class CSceneManager;

private:
	CSceneMain();
	virtual ~CSceneMain();

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();
};

