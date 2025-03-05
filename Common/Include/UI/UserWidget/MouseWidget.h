#pragma once

#include "UserWidget.h"

class CMouseWidget :
    public CUserWidget
{
	friend class CSceneUIManager;
	friend class CRenderManager;

protected:
	CMouseWidget();
	virtual ~CMouseWidget();

protected:
	CSharedPtr<class CImage>	mImage;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

