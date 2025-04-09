#pragma once
#include "Scene/Scene.h"

class CSceneResult : public CScene
{
	friend class CSceneManager;

private:
	CSceneResult();
	virtual ~CSceneResult();
	class CResultWidget* mResultWidget;

protected:
	virtual bool Init() override;
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;

};
