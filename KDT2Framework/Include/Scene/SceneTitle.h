#pragma once
#include "Scene\Scene.h"
class CSceneTitle : public CScene
{
	friend class CSceneManager;

private:
	CSceneTitle();
	virtual ~CSceneTitle();

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();
};

