#include "LobbyWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Share/Log.h"

CLobbyWidget::CLobbyWidget()
{
	mIsHost = false;
	mIsMultiPlay = false;
}

CLobbyWidget::~CLobbyWidget()
{
}

bool CLobbyWidget::Init()
{
	CUserWidget::Init();
	FResolution RS = CDevice::GetInst()->GetResolution();

	mCharacterLeftButton = mScene->GetUIManager()->CreateWidget<CButton>("CharacterLeftButton");
	mCharacterRightButton = mScene->GetUIManager()->CreateWidget<CButton>("CharacterRightButton");

	mMapLeftButton = mScene->GetUIManager()->CreateWidget<CButton>("MapLeftButton");
	mMapRightButton = mScene->GetUIManager()->CreateWidget<CButton>("MapRightButton");

	AddWidget(mCharacterLeftButton);
	AddWidget(mCharacterRightButton);
	AddWidget(mMapLeftButton);
	AddWidget(mMapRightButton);

	mCharacterLeftButton->SetPos(RS.Width * 0.625f, RS.Height * 0.75f);
	mCharacterRightButton->SetPos(RS.Width * 0.875f, RS.Height * 0.75f);
	mMapLeftButton->SetPos(RS.Width * 0.625f, RS.Height * 0.5f);
	mMapRightButton->SetPos(RS.Width * 0.875f, RS.Height * 0.5f);

	FVector2D sizeArrow = FVector2D(100.0f, 100.0f);
	mCharacterLeftButton->SetSize(sizeArrow);
	mCharacterRightButton->SetSize(sizeArrow);
	mMapLeftButton->SetSize(sizeArrow);
	mMapRightButton->SetSize(sizeArrow);

	auto pathLeft = TEXT("Texture\\Icon\\direct-left.png");
	auto pathRight = TEXT("Texture\\Icon\\direct-right.png");

	SetButton(*(mCharacterLeftButton.Get()), "CharacterLeftButton", pathLeft);
	SetButton(*(mCharacterRightButton.Get()), "CharacterRightButton", pathRight);
	SetButton(*(mMapLeftButton.Get()), "MapLeftButton", pathLeft);
	SetButton(*(mMapRightButton.Get()), "MapRightButton", pathRight);

	// 슬롯은 이미지로 가자 -> 투명버튼 -> 이미지(CImage) -> 중간에 이미지 바꾸는거 됨.
	FVector2D slotPosBase = FVector2D(100, 200);
	FVector2D slotPosAdd = FVector2D(120, 0);
	FVector2D slotSize = FVector2D(100.0f, 100.0f);

	for (int i = 0; i < 3; i++)
	{
		CSharedPtr<CButton> slotButton = mScene->GetUIManager()->CreateWidget<CButton>("SlotButton_" + std::to_string(i));

		AddWidget(slotButton);

		mItemSlots.push_back(slotButton);
		slotButton->SetPos(slotPosBase + slotPosAdd * i);
		slotButton->SetSize(slotSize);
		// 차일드 -> 이미지
		// 컬러 0,0,0,0
	}


	return true;
}

void CLobbyWidget::SetButton(CButton& _button, const char* _name, const wchar_t* _path)
{
	FVector3D TintNormal = FVector3D(0, 1, 0);
	FVector3D TintHovered = FVector3D(0, 0.8f, 0);
	FVector3D TintClick = FVector3D(0, 0.6f, 0);

	_button.SetTexture(EButtonState::Normal, _name, _path);
	_button.SetTexture(EButtonState::Hovered, _name, _path);
	_button.SetTexture(EButtonState::Click, _name, _path);
	_button.SetTint(EButtonState::Normal, TintNormal);
	_button.SetTint(EButtonState::Hovered, TintHovered);
	_button.SetTint(EButtonState::Click, TintClick);

}

void CLobbyWidget::CharacterLeftButtonClick()
{
}

void CLobbyWidget::CharacterRightButtonClick()
{
}

void CLobbyWidget::MapLeftButtonClick()
{
}

void CLobbyWidget::MapRightButtonClick()
{
}

void CLobbyWidget::ItemSlotClick(int Index)
{
}

void CLobbyWidget::ItemClick(int Index)
{
}
