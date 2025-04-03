#pragma once
#include "UI\UserWidget\UserWidget.h"
#include "Interface/IObjectNetworkController.h"

class CPlayerObjectWidget: public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CPlayerObjectWidget();
	virtual ~CPlayerObjectWidget();

protected:
	CSharedPtr<class CTextBlock> mInfoText;
	IObjectNetworkController* mObjectNetworkController;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

	void SetObjectNetworkController(IObjectNetworkController* _ctrl) { mObjectNetworkController = _ctrl; }
};

