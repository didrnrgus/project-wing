#pragma once

#include "UI/UserWidget/UserWidget.h"

class CMainWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CMainWidget();
	virtual ~CMainWidget();

protected:
	CSharedPtr<class CInventory>	mInventory;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();

private:
	void InventoryButton();
};

