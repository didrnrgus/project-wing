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

	itemSlotCount = 3;
	itemTypeCount = 3;
	curSelectedSlot = -1;

	mItemImagePaths.push_back(TEXT("Texture\\Icon\\milk.png"));
	mItemImagePaths.push_back(TEXT("Texture\\Icon\\pharagraphspacing.png"));
	mItemImagePaths.push_back(TEXT("Texture\\Icon\\ghost.png"));

	SlotPosBase = FVector2D(100, 100);
	SlotPosAdd = FVector2D(120, 0);
	SlotSize = FVector2D(100.0f, 100.0f);
	SlotInnerItemSizeRate = 0.8f;
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

	// ������ �̹����� ���� -> �����ư -> �̹���(CImage) -> �߰��� �̹��� �ٲٴ°� ��.
	for (int i = 0; i < itemSlotCount; i++)
	{
		FVector2D tempPos = SlotPosBase + SlotPosAdd * i;
		// ���Կ� '+' �ִ°Ͱ� �ƹ��͵� ���� ���� ����Ī �Ұ���.
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>("ButtonBackImage");
		AddWidget(buttonBackImage);
		mItemImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture("ButtonBackImage", TEXT("Texture\\Icon\\add-square.png"));
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(SlotSize);
		buttonBackImage->SetColor(FVector4D::Green);
		buttonBackImage->SetPos(tempPos);

		// ��ư�� ��ǲ �̺�Ʈ�� �ԷµǾ�� �ϹǷ�.
		CSharedPtr<CButton> slotButton = mScene->GetUIManager()->CreateWidget<CButton>("SlotButton_" + std::to_string(i));
		AddWidget(slotButton);
		mItemSlots.push_back(slotButton);
		slotButton->SetSize(SlotSize);
		slotButton->SetPivot(FVector2D::One * 0.5f);
		slotButton->SetColor(FVector4D::Transparent);
		slotButton->SetPos(tempPos);
		slotButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("slotButton index: " + std::to_string(i));
				this->TriggerItemButtons(i);
			});
	}

	// ���� �������� ������ ��ư ���� �ɸ��� �����°� ����.
	// ���̶�Ű ������ ��������.
	for (int i = 0; i < itemTypeCount; i++)
	{
		std::string name = "SelectItemButton_" + std::to_string(i);
		CSharedPtr<CButton> selectItemButton = mScene->GetUIManager()->CreateWidget<CButton>(name);
		AddWidget(selectItemButton);
		mItemButtons.push_back(selectItemButton);
		this->SetButton(*selectItemButton.Get(), name.c_str(), mItemImagePaths[i]);

		selectItemButton->SetPivot(FVector2D::One * 0.5f);
		selectItemButton->SetSize(SlotSize * SlotInnerItemSizeRate);
		selectItemButton->SetEnable(false);
		selectItemButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("selectItemButton index: " + std::to_string(i));
				this->SelectItemForSlot(curSelectedSlot, i);
				this->TriggerItemButtons(-1);
			});
	}

	return true;
}

void CLobbyWidget::SelectItemForSlot(int _slotIndex, int _itemIndex)
{
	asdf
}

/// <summary>
/// ���� Ŭ���ϰ�, ������ N���� �ϳ��� ���������� �ε��� ������. [�ٸ��뵵 -1]
/// </summary>
/// <param name="_index">��� ���� -1��</param>
void CLobbyWidget::TriggerItemButtons(int _index)
{
	curSelectedSlot = _index;

	if (_index == -1)
	{
		for (int i = 0; i < itemTypeCount; i++)
		{
			mItemButtons[i]->SetEnable(false);
		}
		return;
	}

	// �ε��� ���� ������, ������� ���� ���� �ɸ��� ������ ���� -> ���� �����ϰԲ�.
	for (int i = 0; i < itemTypeCount; i++)
	{
		FVector2D tempPos = SlotPosBase + SlotPosAdd * _index // ���� ���� ���� ��ġ -> ����� ���Կ��� ������?
			+ FVector2D(0, SlotSize.y * (i + 1)) * SlotInnerItemSizeRate // ���η� ��ĭ�� �ø���
			+ FVector2D(0, 10) * (i + 1); // ����
		mItemButtons[i]->SetEnable(true);
		mItemButtons[i]->SetPos(tempPos);
	}

}

void CLobbyWidget::SetButton(CButton& _button, const char* _name, const wchar_t* _path)
{
	FVector3D TintNormal = FVector3D(0, 1, 0);
	FVector3D TintHovered = FVector3D(0, SlotInnerItemSizeRate, 0);
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
