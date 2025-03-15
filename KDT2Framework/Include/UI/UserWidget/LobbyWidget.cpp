#include "LobbyWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include <Interface/IPlayerController.h>
#include <Scene/SceneInGame.h>
#include <Scene/SceneTitle.h>
#include "Etc/DataStorageManager.h"

CLobbyWidget::CLobbyWidget()
{
	mIsHost = false;
	mIsMultiPlay = false;
	mSlotTextureNamePrefix = "ButtonBackImage_";
	mItemTextureNamePrefix = "ButtonImage_";
	mSlotButtonNamePrefix = "SlotButton_";
	mMapDifficultyImageNamePrefix = "MapImage_";
	curSelectedSlot = -1;

	curPlayerGraphicIndex = 0;
	curDifficultyIndex = 0;


	for (int i = 0; i < CDataStorageManager::GetInst()->GetMapInfoCount(); i++)
	{
		auto mapInfo = CDataStorageManager::GetInst()->GetMapInfo(i);
		mMapDifficultyImagePaths.push_back(mapInfo.GetIconPath(i));
		mMapDifficultyImageColors.push_back(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
	}

	// 아이템 테이블로 뺴야 함.
	{
		mItemImagePaths.push_back(TEXT("Texture/Icon/milk.png"));
		mItemImagePaths.push_back(TEXT("Texture/Icon/pharagraphspacing.png"));
		mItemImagePaths.push_back(TEXT("Texture/Icon/ghost.png"));

		itemTypeCount = 3;
	}

	mSlotImagePaths.push_back(TEXT("Texture/Icon/add-square.png"));
	mSlotImagePaths.push_back(TEXT("Texture/Icon/empty-square.png"));

	mSlotPosBase = FVector2D(100, 100);
	mSlotPosAdd = FVector2D(120, 0);
	mSlotSize = FVector2D(100.0f, 100.0f);
	mMapDifficultyImagePos = FVector2D(1010, 410);
	mSlotInnerItemSizeRate = 0.8f;
	mMapDifficultySinAngle = 0.0f;
}

CLobbyWidget::~CLobbyWidget()
{
}

bool CLobbyWidget::Init()
{
	CUserWidget::Init();

	InitScrollSelectButtons();
	InitItemButtons();

	// difficulty Image
	mMapDifficultyImage = mScene->GetUIManager()->CreateWidget<CImage>(mMapDifficultyImageNamePrefix);
	AddWidget(mMapDifficultyImage);
	mMapDifficultyImage->SetTexture(mMapDifficultyImageNamePrefix + std::to_string(0)
		, mMapDifficultyImagePaths[0]);
	mMapDifficultyImage->SetPivot(FVector2D::One * 0.5f);
	mMapDifficultyImage->SetSize(FVector2D::One * 128 * 1.0f);
	mMapDifficultyImage->SetPos(mMapDifficultyImagePos);
	mMapDifficultyImage->SetColor(mMapDifficultyImageColors[curDifficultyIndex]);

	// next button
	mNextButton = mScene->GetUIManager()->CreateWidget<CButton>("NextButton");
	AddWidget(mNextButton);
	SetButton(*mNextButton.Get(), "NextButton", TEXT("Texture/Icon/arrow-square-right.png"));
	mNextButton->SetPivot(FVector2D::One * 0.5f);
	mNextButton->SetSize(FVector2D::One * 128 * 1.0f);
	mNextButton->SetPos(FVector2D(1150, 100));
	mNextButton->SetEventCallback(EButtonEventState::Click
		, [this]()
		{
			CLog::PrintLog("mNextButton Click curPlayerGraphicIndex: " + std::to_string(curPlayerGraphicIndex));
			CLog::PrintLog("mNextButton Click curDifficultyIndex: " + std::to_string(curDifficultyIndex));
			CDataStorageManager::GetInst()->SetSelectedCharacterIndex(curPlayerGraphicIndex);
			CDataStorageManager::GetInst()->SetSelectedMapIndex(curDifficultyIndex);
			CSceneManager::GetInst()->CreateLoadScene<CSceneInGame>();
		});

	// previous button
	mPrevButton = mScene->GetUIManager()->CreateWidget<CButton>("PrevButton");
	AddWidget(mPrevButton);
	SetButton(*mPrevButton.Get(), "PrevButton", TEXT("Texture/Icon/arrow-square-left.png"));
	mPrevButton->SetPivot(FVector2D::One * 0.5f);
	mPrevButton->SetSize(FVector2D::One * 96.0f * 1.0f);
	mPrevButton->SetPos(FVector2D(100, 650));
	mPrevButton->SetEventCallback(EButtonEventState::Click
		, []()
		{
			CLog::PrintLog("mPrevButton Click");
			CSceneManager::GetInst()->CreateLoadScene<CSceneTitle>();
		});

	return true;
}

void CLobbyWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	// 맵 선택 이미지 웨이브.
	{
		mMapDifficultySinAngle += DeltaTime * 180.0f;

		if (mMapDifficultySinAngle >= 360.0f)
		{
			mMapDifficultySinAngle = 0.0f;
		}

		float tempVal = sin(DirectX::XMConvertToRadians(mMapDifficultySinAngle));
		mMapDifficultyImage->SetPos(mMapDifficultyImagePos + FVector2D(0.0f, tempVal * 10));
	}
}

void CLobbyWidget::InitScrollSelectButtons()
{
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

	mCharacterLeftButton->SetEventCallback(EButtonEventState::Click, this, &CLobbyWidget::OnCharacterLeftButtonClick);
	mCharacterRightButton->SetEventCallback(EButtonEventState::Click, this, &CLobbyWidget::OnCharacterRightButtonClick);
	mMapLeftButton->SetEventCallback(EButtonEventState::Click, this, &CLobbyWidget::OnMapLeftButtonClick);
	mMapRightButton->SetEventCallback(EButtonEventState::Click, this, &CLobbyWidget::OnMapRightButtonClick);
}

void CLobbyWidget::InitItemButtons()
{
	auto pathLeft = TEXT("Texture/Icon/direct-left.png");
	auto pathRight = TEXT("Texture/Icon/direct-right.png");

	SetButton(*(mCharacterLeftButton.Get()), "CharacterLeftButton", pathLeft);
	SetButton(*(mCharacterRightButton.Get()), "CharacterRightButton", pathRight);
	SetButton(*(mMapLeftButton.Get()), "MapLeftButton", pathLeft);
	SetButton(*(mMapRightButton.Get()), "MapRightButton", pathRight);

	// 슬롯은 이미지로 가자 -> 투명버튼 -> 이미지(CImage) -> 중간에 이미지 바꾸는거 됨.
	for (int i = 0; i < itemSlotCount; i++)
	{
		FVector2D tempPos = mSlotPosBase + mSlotPosAdd * i;
		// 슬롯에 '+' 있는것과 아무것도 없는 슬롯 스위칭 할것임.
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>(mSlotTextureNamePrefix);
		AddWidget(buttonBackImage);
		mItemSlotImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mSlotTextureNamePrefix + std::to_string((int)SlotType::ToAdd)
			, mSlotImagePaths[(int)SlotType::ToAdd]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize);
		buttonBackImage->SetColor(FVector4D::Green);
		buttonBackImage->SetPos(tempPos);

		// 슬롯 내부 아이템 이미지.
		CSharedPtr<CImage> buttonImage = mScene->GetUIManager()->CreateWidget<CImage>(mItemTextureNamePrefix);
		AddWidget(buttonImage);
		mItemImages.push_back(buttonImage);
		buttonImage->SetTexture(mItemTextureNamePrefix + std::to_string(i), mItemImagePaths[i]);
		buttonImage->SetPivot(FVector2D::One * 0.5f);
		buttonImage->SetSize(mSlotSize * mSlotInnerItemSizeRate * mSlotInnerItemSizeRate);
		buttonImage->SetColor(FVector4D::Green);
		buttonImage->SetPos(tempPos);
		buttonImage->SetEnable(false);

		// 버튼이 인풋 이벤트에 입력되어야 하므로.
		CSharedPtr<CButton> slotButton = mScene->GetUIManager()->CreateWidget<CButton>(mSlotButtonNamePrefix + std::to_string(i));
		AddWidget(slotButton);
		mItemSlots.push_back(slotButton);
		slotButton->SetSize(mSlotSize);
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

	// 슬롯 눌렀을때 아이템 버튼 세개 쪼르륵 나오는거 세팅.
	// 하이라키 세팅은 하지말자.
	for (int i = 0; i < itemTypeCount; i++)
	{
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>(mSlotTextureNamePrefix);
		AddWidget(buttonBackImage);
		mItemSlotImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mSlotTextureNamePrefix + std::to_string((int)SlotType::Added)
			, mSlotImagePaths[(int)SlotType::Added]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize * pow(mSlotInnerItemSizeRate, 1));
		buttonBackImage->SetColor(FVector4D::Green);
		buttonBackImage->SetEnable(false);
		mItemButtonSlotImages.push_back(buttonBackImage);

		std::string name = "SelectItemButton_" + std::to_string(i);
		CSharedPtr<CButton> selectItemButton = mScene->GetUIManager()->CreateWidget<CButton>(name);
		AddWidget(selectItemButton);
		mItemButtons.push_back(selectItemButton);
		this->SetButton(*selectItemButton.Get(), name.c_str(), mItemImagePaths[i]);

		selectItemButton->SetPivot(FVector2D::One * 0.5f);
		selectItemButton->SetSize(mSlotSize * pow(mSlotInnerItemSizeRate, 3));
		selectItemButton->SetEnable(false);
		selectItemButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("selectItemButton index: " + std::to_string(i));
				this->SelectItemForSlot(curSelectedSlot, i);
				this->TriggerItemButtons(-1);
			});
	}
}

void CLobbyWidget::SelectItemForSlot(int _slotIndex, int _itemIndex)
{
	// 슬롯 내 아이템 이미지가 변경이 되어야 함. 
	CLog::PrintLog("SelectItemForSlot");
	CLog::PrintLog("_slotIndex: " + std::to_string(_slotIndex));
	CLog::PrintLog("_itemIndex: " + std::to_string(_itemIndex));

	auto slotImage = mItemSlotImages[_slotIndex];
	slotImage->SetTexture(mSlotTextureNamePrefix + std::to_string((int)SlotType::Added)
		, mSlotImagePaths[(int)SlotType::Added]);

	auto itemImage = mItemImages[_slotIndex];
	itemImage->SetTexture(mItemTextureNamePrefix + std::to_string(_itemIndex)
		, mItemImagePaths[_itemIndex]);
	itemImage->SetEnable(true);
}

/// <summary>
/// 슬롯 클릭하고, 아이템 N개중 하나를 선택했을떄 인덱스 보내기. [다른용도 -1]
/// </summary>
/// <param name="_index">모두 끌땐 -1로</param>
void CLobbyWidget::TriggerItemButtons(int _index)
{
	curSelectedSlot = _index;

	if (_index == -1)
	{
		for (int i = 0; i < itemTypeCount; i++)
		{
			mItemButtons[i]->SetEnable(false);
			mItemButtonSlotImages[i]->SetEnable(false);
		}
		return;
	}

	// 인덱스 들어온 지점에, 순서대로 세로 위로 쪼르륵 아이템 나열 -> 선택 가능하게끔.
	for (int i = 0; i < itemTypeCount; i++)
	{
		FVector2D tempPos = mSlotPosBase + FVector2D::Axis[EAxis::Y] * 10 + mSlotPosAdd * _index // 가로 슬롯 기준 위치 -> 몇번쨰 슬롯에서 눌렸지?
			+ FVector2D(0, mSlotSize.y * (i + 1)) * mSlotInnerItemSizeRate // 세로로 한칸씩 올릴꺼
			+ FVector2D(0, 5) * (i + 1); // 간격
		mItemButtons[i]->SetEnable(true);
		mItemButtons[i]->SetPos(tempPos);
		mItemButtonSlotImages[i]->SetEnable(true);
		mItemButtonSlotImages[i]->SetPos(tempPos);
	}

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

void CLobbyWidget::OnCharacterLeftButtonClick()
{
	CLog::PrintLog("CharacterLeftButtonClick()");
	auto playerController = dynamic_cast<IPlayerController*>(mScene);

	if (playerController)
	{
		curPlayerGraphicIndex++;

		if (curPlayerGraphicIndex == CDataStorageManager::GetInst()->GetCharacterCount())
			curPlayerGraphicIndex = 0;

		playerController->SetChangeGraphic(0, curPlayerGraphicIndex);
	}
}

void CLobbyWidget::OnCharacterRightButtonClick()
{
	CLog::PrintLog("CharacterRightButtonClick()");
	auto playerController = dynamic_cast<IPlayerController*>(mScene);

	if (playerController)
	{
		if (curPlayerGraphicIndex > 0)
			curPlayerGraphicIndex--;
		else
			curPlayerGraphicIndex = CDataStorageManager::GetInst()->GetCharacterCount() - 1;

		playerController->SetChangeGraphic(0, curPlayerGraphicIndex);
	}
}

void CLobbyWidget::OnMapLeftButtonClick()
{
	CLog::PrintLog("MapLeftButtonClick()");

	curDifficultyIndex--;

	if (curDifficultyIndex < 0)
		curDifficultyIndex = CDataStorageManager::GetInst()->GetMapInfoCount() - 1;

	mMapDifficultyImage->SetTexture(mMapDifficultyImageNamePrefix + std::to_string(curDifficultyIndex)
		, mMapDifficultyImagePaths[curDifficultyIndex]);
	mMapDifficultyImage->SetColor(mMapDifficultyImageColors[curDifficultyIndex]);
}

void CLobbyWidget::OnMapRightButtonClick()
{
	CLog::PrintLog("MapRightButtonClick()");

	curDifficultyIndex++;

	if (curDifficultyIndex == CDataStorageManager::GetInst()->GetMapInfoCount())
		curDifficultyIndex = 0;

	mMapDifficultyImage->SetTexture(mMapDifficultyImageNamePrefix + std::to_string(curDifficultyIndex)
		, mMapDifficultyImagePaths[curDifficultyIndex]);
	mMapDifficultyImage->SetColor(mMapDifficultyImageColors[curDifficultyIndex]);
}

