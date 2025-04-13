#pragma once
#include "Scene/Scene.h"

class CSceneRank:public CScene
{
	friend class CSceneManager;

private:
	class CRankWidget* mRankWidget;

private:
	CSceneRank();
	virtual ~CSceneRank();

protected:
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;

};
