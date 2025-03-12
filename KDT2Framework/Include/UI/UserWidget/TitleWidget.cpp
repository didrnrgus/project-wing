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
	FVector2D size = FVector2D(200.0f, 100.0f);
	FVector2D singlePos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 4) - size * 0.5f;
	FVector2D multiPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 3) - size * 0.5f;
	FVector2D rankPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 2) - size * 0.5f;
	FVector2D exitPos = FVector2D(RS.Width * 0.5f, RS.Height * 0.2f * 1) - size * 0.5f;

	SetButtonWithTextBlock(mSinglePlayButton, "SinglePlay", singlePos
		, &CTitleWidget::SinglePlayButtonClick, mSinglePlayTextBlock, TEXT("SinglePlay"));
	SetButtonWithTextBlock(mMultiPlayButton, "MultiPlay", multiPos
		, &CTitleWidget::MultiPlayButtonClick, mMultiPlayTextBlock, TEXT("MultiPlay"));
	SetButtonWithTextBlock(mRankButton, "Rank", rankPos
		, &CTitleWidget::RankButtonClick, mRankTextBlock, TEXT("Rank"));
	SetButtonWithTextBlock(mExitButton, "Exit", exitPos
		, &CTitleWidget::ExitButtonClick, mExitTextBlock, TEXT("Exit"));

	return true;
}

void CTitleWidget::SetButtonWithTextBlock(CButton* button, std::string name, FVector2D pos
	, void(CTitleWidget::* Func)(), CTextBlock* textBlock, const wchar_t* textBlockContent)
{
	button = mScene->GetUIManager()->CreateWidget<CButton>(name + "Button");
	AddWidget(button);
	button->SetPos(pos);
	button->SetSize(200.f, 100.f);
	button->SetColor(0, 0, 0, 0);
	button->SetEventCallback<CTitleWidget>(EButtonEventState::Click, this, Func);
	textBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>(name + "Text");
	button->SetChild(textBlock);
	textBlock->SetText(textBlockContent);
	textBlock->SetTextColor(0, 255, 0, 255);
	textBlock->SetAlignH(ETextAlignH::Center);
	textBlock->SetFontSize(30.f);
	textBlock->SetShadowEnable(true);
	textBlock->SetShadowOffset(3.f, 3.f);
	textBlock->SetTextShadowColor(FVector4D(0.5f, 0.5f, 0.5f, 1.0f));
}

void CTitleWidget::LoadData()
{

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

void CTitleWidget::RankButtonClick()
{
	// Move Rank Scene 
	CLog::PrintLog("CTitleWidget::RankButtonClick()");
}

void CTitleWidget::ExitButtonClick()
{
	CLog::PrintLog("CTitleWidget::ExitButtonClick()");
	CGameManager::GetInst()->ExitGame();
}
