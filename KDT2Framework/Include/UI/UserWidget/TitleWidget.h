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
	CSharedPtr<class CButton>	mRankButton;
	CSharedPtr<class CButton>	mExitButton;

	CSharedPtr<class CTextBlock>	mSinglePlayTextBlock;
	CSharedPtr<class CTextBlock>	mMultiPlayTextBlock;
	CSharedPtr<class CTextBlock>	mRankTextBlock;
	CSharedPtr<class CTextBlock>	mExitTextBlock;

public:
	virtual bool Init() override;

private:
	void SetButtonWithTextBlock(class CButton* button, std::string name, FVector2D pos
		, void(CTitleWidget::* Func)(), class CTextBlock* textBlock, const wchar_t* textBlockContent);
	void SinglePlayButtonClick();
	void MultiPlayButtonClick();
	void RankButtonClick();
	void ExitButtonClick();
};

