#include "StartWidget.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneMain.h"
#include "Scene/SceneEditor.h"
#include "Device.h"
#include "GameManager.h"

CStartWidget::CStartWidget()
{
}

CStartWidget::~CStartWidget()
{
}

bool CStartWidget::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	CSharedPtr<CImage>  Back = mScene->GetUIManager()->CreateWidget<CImage>("Back");

	Back->SetTexture("StartBack", TEXT("Texture\\Back.png"));
	Back->SetSize((float)RS.Width, (float)RS.Height);

	AddWidget(Back);

	mStartButton = mScene->GetUIManager()->CreateWidget<CButton>("StartButton");
	mEditorButton = mScene->GetUIManager()->CreateWidget<CButton>("EditorButton");
	mExitButton = mScene->GetUIManager()->CreateWidget<CButton>("ExitButton");
	AddWidget(mStartButton);
	AddWidget(mExitButton);
	AddWidget(mEditorButton);

	mStartButton->SetPos(RS.Width * 0.5f - 100.f, RS.Height * 0.5f + 100.f);
	mEditorButton->SetPos(RS.Width * 0.5f - 100.f, RS.Height * 0.5f - 50.f);
	mExitButton->SetPos(RS.Width * 0.5f - 100.f, RS.Height * 0.5f - 200.f);

	mExitButton->SetSize(200.f, 100.f);
	mEditorButton->SetSize(200.f, 100.f);
	mStartButton->SetSize(200.f, 100.f);
	//Button->SetTint(EButtonState::Normal, 1.f, 0.f, 0.f);
	mStartButton->SetTexture(EButtonState::Normal, "StartButton", TEXT("Texture\\Start.png"));
	mStartButton->SetTexture(EButtonState::Hovered, "StartButton", TEXT("Texture\\Start.png"));
	mStartButton->SetTexture(EButtonState::Click, "StartButton", TEXT("Texture\\Start.png"));

	mEditorButton->SetTexture(EButtonState::Normal, "StartButton", TEXT("Texture\\Start.png"));
	mEditorButton->SetTexture(EButtonState::Hovered, "StartButton", TEXT("Texture\\Start.png"));
	mEditorButton->SetTexture(EButtonState::Click, "StartButton", TEXT("Texture\\Start.png"));

	mExitButton->SetTexture(EButtonState::Normal, "EndButton", TEXT("Texture\\End.png"));
	mExitButton->SetTexture(EButtonState::Hovered, "EndButton", TEXT("Texture\\End.png"));
	mExitButton->SetTexture(EButtonState::Click, "EndButton", TEXT("Texture\\End.png"));

	mStartButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mStartButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mStartButton->SetTint(EButtonState::Click, 0.6f, 0.6f, 0.6f);

	mEditorButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mEditorButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mEditorButton->SetTint(EButtonState::Click, 0.6f, 0.6f, 0.6f);

	mExitButton->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	mExitButton->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	mExitButton->SetTint(EButtonState::Click, 0.6f, 0.6f, 0.6f);

	mStartButton->SetEventCallback<CStartWidget>(EButtonEventState::Click, this, &CStartWidget::StartButtonClick);
	mEditorButton->SetEventCallback<CStartWidget>(EButtonEventState::Click, this, &CStartWidget::EditorButtonClick);
	mExitButton->SetEventCallback<CStartWidget>(EButtonEventState::Click, this, &CStartWidget::ExitButtonClick);

	return true;
}

void CStartWidget::StartButtonClick()
{
	CSceneManager::GetInst()->CreateLoadScene<CSceneMain>();
}

void CStartWidget::EditorButtonClick()
{
	CSceneManager::GetInst()->CreateLoadScene<CSceneEditor>();
}

void CStartWidget::ExitButtonClick()
{
	CGameManager::GetInst()->ExitGame();
}
