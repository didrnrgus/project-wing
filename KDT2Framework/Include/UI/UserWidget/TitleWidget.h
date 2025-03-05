#pragma once
#include "UI/UserWidget/UserWidget.h"

class CTitleWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CTitleWidget();
	virtual ~CTitleWidget();

protected:
	CSharedPtr<class CButton>	mSinglePlayButton;
	CSharedPtr<class CButton>	mMultiPlayButton;
	CSharedPtr<class CButton>	mExitButton;

public:
	virtual bool Init();

private:
	void SinglePlayButtonClick();
	void MultiPlayButtonClick();
	void ExitButtonClick();
};

