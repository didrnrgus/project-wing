#include "MainWidget.h"
#include "UI/Common/Button.h"
#include "UI/Common/TextBlock.h"
#include "UI/Common/ProgressBar.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Device.h"
#include "UI/UserWidget/Inventory.h"

CMainWidget::CMainWidget()
{
}

CMainWidget::~CMainWidget()
{
}

bool CMainWidget::Init()
{
	CUserWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();

	mInventory = mScene->GetUIManager()->CreateWidget<CInventory>("Inventory");

	mInventory->SetZOrder(10);
	mInventory->SetPos(100.f, 100.f);

	AddWidget(mInventory);

	CSharedPtr<CButton>    Button = mScene->GetUIManager()->CreateWidget<CButton>("TestButton");

	Button->SetPos(400.f, 100.f);
	Button->SetSize(200.f, 100.f);
	//Button->SetTint(EButtonState::Normal, 1.f, 0.f, 0.f);
	Button->SetTexture(EButtonState::Normal, "StartButton", TEXT("Texture\\Start.png"));
	Button->SetTexture(EButtonState::Hovered, "StartButton", TEXT("Texture\\Start.png"));
	Button->SetTexture(EButtonState::Click, "StartButton", TEXT("Texture\\Start.png"));

	Button->SetTint(EButtonState::Normal, 0.8f, 0.8f, 0.8f);
	Button->SetTint(EButtonState::Hovered, 1.f, 1.f, 1.f);
	Button->SetTint(EButtonState::Click, 0.6f, 0.6f, 0.6f);

	Button->SetSound(EButtonEventState::Hovered, "ButtonHovered", "Sound\\TeemoSmile.mp3");
	Button->SetSound(EButtonEventState::Click, "ButtonClick", "Sound\\TeemoStartClicck.mp3");

	Button->SetEventCallback<CMainWidget>(EButtonEventState::Click, this, &CMainWidget::InventoryButton);

	AddWidget(Button);

	CSharedPtr<CTextBlock> Text = mScene->GetUIManager()->CreateWidget<CTextBlock>("TestButtonText");

	Text->SetText(TEXT("Buttonspkjghlwkehrolih"));
	Text->SetTextColor(255, 0, 0, 255);
	//Text->SetPos(100.f, 100.f);
	//Text->SetSize(100.f, 100.f);
	Text->SetAlignH(ETextAlignH::Center);
	Text->SetFontSize(60.f);
	Text->SetShadowEnable(true);
	Text->SetShadowOffset(3.f, 3.f);

	Button->SetChild(Text);

	CSharedPtr<CProgressBar>    HPBar = mScene->GetUIManager()->CreateWidget<CProgressBar>("HPBar");

	HPBar->SetPos(50.f, RS.Height - 300.f);
	//HPBar->SetSize(50.f, 200.f);
	HPBar->SetSize(200.f, 50.f);
	HPBar->SetTint(EProgressBarImageType::Back, 0.3f, 0.3f, 0.3f);
	//HPBar->SetOpacity(EProgressBarImageType::Fill, 0.4f);
	HPBar->SetTexture(EProgressBarImageType::Fill, "HPBar", TEXT("Texture\\HPBar.png"));
	//HPBar->SetBarDir(EProgressBarDir::TopToBottom);

	HPBar->SetPercent(0.7f);

	AddWidget(HPBar);

	return true;
}

void CMainWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);
}

void CMainWidget::Render()
{
	CUserWidget::Render();
}

void CMainWidget::InventoryButton()
{
	if (mInventory->IsEnable())
		mInventory->SetEnable(false);

	else
		mInventory->SetEnable(true);
}
