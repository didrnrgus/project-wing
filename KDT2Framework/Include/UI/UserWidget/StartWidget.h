#pragma once

#include "UI/UserWidget/UserWidget.h"

class CStartWidget :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CStartWidget();
	virtual ~CStartWidget();

protected:
	CSharedPtr<class CButton>	mStartButton;
	CSharedPtr<class CButton>	mEditorButton;
	CSharedPtr<class CButton>	mExitButton;

public:
	virtual bool Init();

private:
	void StartButtonClick();
	void EditorButtonClick();
	void ExitButtonClick();
};

