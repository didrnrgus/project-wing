#pragma once
#include "UI/UserWidget/SceneWidget.h"
#include "Interface/IObjectNetworkController.h"

class CTitleWidget : public CSceneWidget, public IObjectNetworkController
{
	friend class CSceneUIManager;

protected:
	CTitleWidget();
	virtual ~CTitleWidget();

protected:
	enum ETitleUIType
	{
		SinglePlay = 0,
		MultiPlay,
		Rank,
		Exit,
		End
	};

private:
	float mCurReConnectionTime;
	float mMaxReConnectionTime;

	int mWaitTime;
	int mIsSkip;

protected:
	CSharedPtr<class CTextBlock> mTitleText;

	CSharedPtr<class CButton> mSinglePlayButton;
	CSharedPtr<class CButton> mMultiPlayButton;
	CSharedPtr<class CButton> mRankButton;
	CSharedPtr<class CButton> mExitButton;

	CSharedPtr<class CTextBlock> mSinglePlayTextBlock;
	CSharedPtr<class CTextBlock> mMultiPlayTextBlock;
	CSharedPtr<class CTextBlock> mRankTextBlock;
	CSharedPtr<class CTextBlock> mExitTextBlock;
	std::vector<CSharedPtr<class CTextBlock>> mArrTextBlock;

	FVector2D mResolution;
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

private:
	void SetButtonWithTextBlock(ETitleUIType _type
		, CSharedPtr<class CButton>& button
		, std::string name
		, FVector2D pos
		, void(CTitleWidget::* ClickCallbackFunc)()
		, CSharedPtr<class CTextBlock>& textBlock
		, const wchar_t* textBlockContent
	);

	void LoadGameData();
	void LoadRankData();
	void LoadProcess();

	void SinglePlayButtonClick();
	void MultiPlayButtonClick();
	void RankButtonClick();
	void ExitButtonClick();

	virtual void AddListener() override;
	virtual void RemoveListener() override;
	virtual void ProcessMessage(const struct RecvMessage& msg) override;
};

