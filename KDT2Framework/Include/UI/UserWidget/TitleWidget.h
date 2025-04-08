#pragma once
#include "UI/UserWidget/SceneWidget.h"
#include "Interface/IObjectNetworkController.h"

class CTitleWidget : public CSceneWidget, public IObjectNetworkController
{
	friend class CSceneUIManager;

protected:
	CTitleWidget();
	virtual ~CTitleWidget();

private:
	int mTaskID;
	float mCurReConnectionTime;
	float mMaxReConnectionTime;
	
	int mWaitTime;
	int mIsSkip;

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
	virtual void Update(float DeltaTime) override;

private:
	void SetButtonWithTextBlock(CSharedPtr<class CButton>& button, std::string name, FVector2D pos
		, void(CTitleWidget::* Func)(), CSharedPtr<class CTextBlock>& textBlock, const wchar_t* textBlockContent);
	
	void LoadGameData();
	void LoadRankData();
	void LoadProcess();
	void LoadScene();
	
	void SinglePlayButtonClick();
	void MultiPlayButtonClick();
	void RankButtonClick();
	void ExitButtonClick();

	virtual void AddListener() override;
	virtual void RemoveListener() override;
	virtual void ProcessMessage(const struct RecvMessage& msg) override;
};

