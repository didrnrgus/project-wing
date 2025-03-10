#include "TitleWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Share/Log.h"

CTitleWidget::CTitleWidget()
{
}

CTitleWidget::~CTitleWidget()
{
}

bool CTitleWidget::Init()
{
	CUserWidget::Init();
	FResolution RS = CDevice::GetInst()->GetResolution();

	mSinglePlayButton = mScene->GetUIManager()->CreateWidget<CButton>("SinglePlayButton");
	mMultiPlayButton = mScene->GetUIManager()->CreateWidget<CButton>("MultiPlayButton");
	mExitButton = mScene->GetUIManager()->CreateWidget<CButton>("ExitButton");

	AddWidget(mSinglePlayButton);
	AddWidget(mExitButton);
	AddWidget(mMultiPlayButton);

	mSinglePlayButton->SetPos(RS.Width * 0.5f - 100.f, RS.Height * 0.5f + 100.f);
	mMultiPlayButton->SetPos(RS.Width * 0.5f - 100.f, RS.Height * 0.5f - 50.f);
	mExitButton->SetPos(RS.Width * 0.5f - 100.f, RS.Height * 0.5f - 200.f);

	mExitButton->SetSize(200.f, 100.f);
	mMultiPlayButton->SetSize(200.f, 100.f);
	mSinglePlayButton->SetSize(200.f, 100.f);

	CSharedPtr<CTextBlock> SinglePlayText = mScene->GetUIManager()->CreateWidget<CTextBlock>("SinglePlayText");
	SinglePlayText->SetText(TEXT("SinglePlay"));
	SinglePlayText->SetTextColor(0, 255, 0, 255);
	SinglePlayText->SetAlignH(ETextAlignH::Center);
	SinglePlayText->SetFontSize(30.f);
	SinglePlayText->SetShadowEnable(true);
	SinglePlayText->SetShadowOffset(3.f, 3.f);
	SinglePlayText->SetTextShadowColor(FVector4D(0.5f, 0.5f, 0.5f, 1.0f));
	mSinglePlayButton->SetChild(SinglePlayText);
	mSinglePlayButton->SetColor(0, 0, 0, 0);

	CSharedPtr<CTextBlock> MultiPlayText = mScene->GetUIManager()->CreateWidget<CTextBlock>("MultiPlayText");
	MultiPlayText->SetText(TEXT("MultiPlay"));
	MultiPlayText->SetTextColor(0, 255, 0, 255);
	MultiPlayText->SetAlignH(ETextAlignH::Center);
	MultiPlayText->SetFontSize(30.f);
	MultiPlayText->SetShadowEnable(true);
	MultiPlayText->SetShadowOffset(3.f, 3.f);
	MultiPlayText->SetTextShadowColor(FVector4D(0.5f, 0.5f, 0.5f, 1.0f));
	mMultiPlayButton->SetChild(MultiPlayText);
	mMultiPlayButton->SetColor(0, 0, 0, 0);

	CSharedPtr<CTextBlock> ExitText = mScene->GetUIManager()->CreateWidget<CTextBlock>("ExitText");
	ExitText->SetText(TEXT("Exit"));
	ExitText->SetTextColor(0, 255, 0, 255);
	ExitText->SetAlignH(ETextAlignH::Center);
	ExitText->SetFontSize(30.f);
	ExitText->SetShadowEnable(true);
	ExitText->SetShadowOffset(3.f, 3.f);
	ExitText->SetTextShadowColor(FVector4D(0.5f, 0.5f, 0.5f, 1.0f));
	mExitButton->SetChild(ExitText);
	mExitButton->SetColor(0, 0, 0, 0);

	mSinglePlayButton->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, &CTitleWidget::SinglePlayButtonClick);
	mMultiPlayButton->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, &CTitleWidget::MultiPlayButtonClick);
	mExitButton->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, &CTitleWidget::ExitButtonClick);
	
	return true;
}

void CTitleWidget::SinglePlayButtonClick()
{
	// 로비로 가야 함.
	CLog::PrintLog("CTitleWidget::SinglePlayButtonClick()");
	CSceneManager::GetInst()->CreateLoadScene<CSceneLobby>();
}

void CTitleWidget::MultiPlayButtonClick()
{
	// 로비로 가야 함.
	CLog::PrintLog("CTitleWidget::MultiPlayButtonClick()");
}

void CTitleWidget::ExitButtonClick()
{
	CLog::PrintLog("CTitleWidget::ExitButtonClick()");
	CGameManager::GetInst()->ExitGame();
}
