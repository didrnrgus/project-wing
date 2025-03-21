#include "LobbyWidget.h"
#include "Device.h"
#include "GameManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Interface/IScenePlayerGraphicController.h"
#include "Scene/SceneInGame.h"
#include "Scene/SceneTitle.h"
#include "Etc/DataStorageManager.h"
#include "Etc/ConstValues.h"
#include "Etc/ZOrderContainer.h"

CLobbyWidget::CLobbyWidget()
{
	mIsHost = false;
	mIsMultiPlay = false;
	/*mSlotTextureNamePrefix = "ButtonBackImage_";
	mItemTextureNamePrefix = "ButtonImage_";
	mSlotButtonNamePrefix = "SlotButton_";
	mMapDifficultyImageNamePrefix = "MapImage_";*/
	curSelectedSlot = -1;

	curPlayerGraphicIndex = 0;
	curDifficultyIndex = 0;

	FResolution RS = CDevice::GetInst()->GetResolution();
	mResolution = FVector2D(RS.Width, RS.Height);

	for (int i = 0; i < CDataStorageManager::GetInst()->GetMapInfoCount(); i++)
	{
		auto mapInfo = CDataStorageManager::GetInst()->GetMapInfo(i);
		mMapDifficultyImagePaths.push_back(mapInfo.GetIconPath(i));
		mMapDifficultyImageNames.push_back(mapInfo.GetIconName(i));
		mMapDifficultyImageColors.push_back(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
	}

	{ // 아이템 정보 세팅.
		auto items = CDataStorageManager::GetInst()->GetItemInfoDatas();
		int index = 0;
		for (auto item : items)
		{
			mItemImagePaths.push_back(item.second.GetItmeImagePath(index));
			mItemImageNames.push_back(item.second.GetItmeImageName(index));
			index++;
		}

		itemSlotCount = CDataStorageManager::GetInst()->GetSelectableItemCount();
		itemTypeCount = mItemImagePaths.size();
	}

	mSlotImagePaths.push_back(ITEM_ADD_SQUARE_PATH);
	mSlotImagePaths.push_back(ITEM_EMPTY_SQUARE_PATH);
	mSlotImageNames.push_back(ITEM_ADD_SQUARE_NAME);
	mSlotImageNames.push_back(ITEM_EMPTY_SQUARE_NAME);

	mSlotPosBase = FVector2D(100, 100);
	mSlotPosAdd = FVector2D(120, 0);
	mSlotSize = FVector2D(100.0f, 100.0f);
	mMapDifficultyImagePos = FVector2D(mResolution.x * 0.8f, mResolution.y * 0.4f);
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
	InitDifficultiImage();
	InitNextPrevButton();
	InitPlayerStatText();
	InitMapInfoText();
	InitItemInfoTooltip();

	auto pathLeft = DIRECT_LEFT_PATH;
	auto pathRight = DIRECT_RIGHT_PATH;

	SetButton(*(mCharacterLeftButton.Get()), "CharacterLeftButton", pathLeft);
	SetButton(*(mCharacterRightButton.Get()), "CharacterRightButton", pathRight);
	SetButton(*(mMapLeftButton.Get()), "MapLeftButton", pathLeft);
	SetButton(*(mMapRightButton.Get()), "MapRightButton", pathRight);


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
	mCharacterLeftButton = mScene->GetUIManager()->CreateWidget<CButton>("CharacterLeftButton");
	mCharacterRightButton = mScene->GetUIManager()->CreateWidget<CButton>("CharacterRightButton");

	mMapLeftButton = mScene->GetUIManager()->CreateWidget<CButton>("MapLeftButton");
	mMapRightButton = mScene->GetUIManager()->CreateWidget<CButton>("MapRightButton");

	AddWidget(mCharacterLeftButton);
	AddWidget(mCharacterRightButton);
	AddWidget(mMapLeftButton);
	AddWidget(mMapRightButton);

	mCharacterLeftButton->SetPos(mResolution.x * 0.7f, mResolution.y * 0.75f);
	mCharacterRightButton->SetPos(mResolution.x * 0.9f, mResolution.y * 0.75f);
	mMapLeftButton->SetPos(mResolution.x * 0.7f, mResolution.y * 0.4f);
	mMapRightButton->SetPos(mResolution.x * 0.9f, mResolution.y * 0.4f);

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

void CLobbyWidget::InitPlayerStatText()
{
	int count = (int)ECharacterStatText::End;
	mPlayerStatNameText.resize(count);
	mPlayerStatValueText.resize(count);
	FVector2D nameBasePos = FVector2D(510.0f, mResolution.y * 0.8f);
	FVector2D valueBasePos = FVector2D(670.0f, mResolution.y * 0.8f);
	FVector2D textSize = FVector2D(200.0f, 100.0f);
	FVector2D nameBasePivot = FVector2D(0.0f, 0.0f);
	FVector2D valueBasePivot = FVector2D(0.0f, 0.0f);
	float fontSize = 30.0f;
	FCharacterState stat = CDataStorageManager::GetInst()->GetCharacterState(curPlayerGraphicIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(ECharacterStatText::gArrCharacterStatText[i]) + L" :";
		auto valueText = stat.GetStatToWString(static_cast<ECharacterStatText::Type>(i));
		auto textBlockName = mScene->GetUIManager()->CreateWidget<CTextBlock>("playerStatName_" + std::to_string(i));
		auto textBlockValue = mScene->GetUIManager()->CreateWidget<CTextBlock>("playerStatValue_" + std::to_string(i));

		AddWidget(textBlockName);
		textBlockName->SetPivot(nameBasePivot);
		textBlockName->SetSize(textSize);
		textBlockName->SetPos(nameBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 10.0f) * i);
		textBlockName->SetText(nameText.c_str());
		textBlockName->SetTextColor(FVector4D::Green);
		textBlockName->SetAlignH(ETextAlignH::Left);
		textBlockName->SetFontSize(fontSize);
		textBlockName->SetShadowEnable(true);
		textBlockName->SetShadowOffset(3.f, 3.f);
		textBlockName->SetTextShadowColor(FVector4D::Gray30);
		mPlayerStatNameText[i] = textBlockName;

		AddWidget(textBlockValue);
		textBlockValue->SetPivot(valueBasePivot);
		textBlockValue->SetSize(textSize);
		textBlockValue->SetPos(valueBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 10.0f) * i);
		textBlockValue->SetText(valueText.c_str());
		textBlockValue->SetTextColor(FVector4D::Green);
		textBlockValue->SetAlignH(ETextAlignH::Left);
		textBlockValue->SetFontSize(fontSize);
		textBlockValue->SetShadowEnable(true);
		textBlockValue->SetShadowOffset(3.f, 3.f);
		textBlockValue->SetTextShadowColor(FVector4D::Gray30);
		mPlayerStatValueText[i] = textBlockValue;
	}
}

void CLobbyWidget::InitMapInfoText()
{
	int count = (int)EMapInfoText::End;
	mMapInfoNameText.resize(count);
	mMapInfoValueText.resize(count);
	FVector2D nameBasePos = FVector2D(510.0f, mResolution.y * 0.4f);
	FVector2D valueBasePos = FVector2D(670.0f, mResolution.y * 0.4f);
	FVector2D textSize = FVector2D(200.0f, 100.0f);
	FVector2D nameBasePivot = FVector2D(0.0f, 0.0f);
	FVector2D valueBasePivot = FVector2D(0.0f, 0.0f);
	float fontSize = 30.0f;
	FMapInfo info = CDataStorageManager::GetInst()->GetMapInfo(curDifficultyIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(EMapInfoText::gArrMapInfoText[i]) + L" :";
		auto valueText = info.GetInfoToWString(static_cast<EMapInfoText::Type>(i));
		auto textBlockName = mScene->GetUIManager()->CreateWidget<CTextBlock>("mapInfoName_" + std::to_string(i));
		auto textBlockValue = mScene->GetUIManager()->CreateWidget<CTextBlock>("mapInfoValue_" + std::to_string(i));

		AddWidget(textBlockName);
		textBlockName->SetPivot(nameBasePivot);
		textBlockName->SetSize(textSize);
		textBlockName->SetPos(nameBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 15.0f) * i);
		textBlockName->SetText(nameText.c_str());
		textBlockName->SetTextColor(FVector4D::Green);
		textBlockName->SetAlignH(ETextAlignH::Left);
		textBlockName->SetFontSize(fontSize);
		textBlockName->SetShadowEnable(true);
		textBlockName->SetShadowOffset(3.f, 3.f);
		textBlockName->SetTextShadowColor(FVector4D::Gray30);
		mMapInfoNameText[i] = textBlockName;

		AddWidget(textBlockValue);
		textBlockValue->SetPivot(valueBasePivot);
		textBlockValue->SetSize(textSize);
		textBlockValue->SetPos(valueBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 15.0f) * i);
		textBlockValue->SetText(valueText.c_str());
		textBlockValue->SetTextColor(FVector4D::Green);
		textBlockValue->SetAlignH(ETextAlignH::Left);
		textBlockValue->SetFontSize(fontSize);
		textBlockValue->SetShadowEnable(true);
		textBlockValue->SetShadowOffset(3.f, 3.f);
		textBlockValue->SetTextShadowColor(FVector4D::Gray30);
		mMapInfoValueText[i] = textBlockValue;
	}
}

void CLobbyWidget::InitItemInfoTooltip()
{
	// 아이템 1개를 타겟으로 잡고 툴팁 띄우는거다.
	// button hover 되었을때 나오게끔.
	FVector2D backImagePos = FVector2D(640.0f, 360.0f);
	FVector2D backImagePivot = FVector2D(0.0f, 0.5f);
	FVector2D backImageSize = FVector2D(340.0f, 155.0f);
	FVector2D back2ImagePos = backImagePos + FVector2D(-5.0f, 0.0f);
	FVector2D back2ImageSize = backImageSize + FVector2D(1.0f, 1.0f) * 10.0f;
	FVector2D backShadowImagePos = back2ImagePos + FVector2D(10.0f, -10.0f);
	FVector2D backShadowImageSize = back2ImageSize;

	mToolTipShasdowImage = mScene->GetUIManager()->CreateWidget<CImage>("mToolTipShasdowImage");
	AddWidget(mToolTipShasdowImage);
	mToolTipShasdowImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mToolTipShasdowImage->SetPivot(backImagePivot);
	mToolTipShasdowImage->SetSize(backShadowImageSize);
	mToolTipShasdowImage->SetColor(FVector4D::Gray30);
	mToolTipShasdowImage->SetPos(backShadowImagePos);
	mToolTipShasdowImage->SetZOrder(ZORDER_LOBBY_TOOLTIP_SHADOW);

	mToolTipBack2Image = mScene->GetUIManager()->CreateWidget<CImage>("mToolTipBack2Image");
	AddWidget(mToolTipBack2Image);
	mToolTipBack2Image->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mToolTipBack2Image->SetPivot(backImagePivot);
	mToolTipBack2Image->SetSize(back2ImageSize);
	mToolTipBack2Image->SetColor(FVector4D::Green);
	mToolTipBack2Image->SetPos(back2ImagePos);
	mToolTipBack2Image->SetZOrder(ZORDER_LOBBY_TOOLTIP_BACK);

	mToolTipBackImage = mScene->GetUIManager()->CreateWidget<CImage>("mToolTipBackImage");
	AddWidget(mToolTipBackImage);
	mToolTipBackImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mToolTipBackImage->SetPivot(backImagePivot);
	mToolTipBackImage->SetSize(backImageSize);
	mToolTipBackImage->SetColor(FVector4D::Black * 0.9f);
	mToolTipBackImage->SetPos(backImagePos);
	mToolTipBackImage->SetZOrder(ZORDER_LOBBY_TOOLTIP);

	mItemInfoNameText.resize(EItemInfoText::End);
	mItemInfoValueText.resize(EItemInfoText::End);

	FVector2D namePos = backImagePos
		+ FVector2D(backImageSize.x * backImagePivot.x, backImageSize.y * backImagePivot.y)
		+ FVector2D(1.f, -1.f * 0.0f) * 5.0f;
	FVector2D namePivot = FVector2D(0.0f, 1.0f);
	FVector2D nameSize = FVector2D(100.0f, 30.0f);
	FVector2D valuePos = namePos + FVector2D::Axis[EAxis::X] * nameSize.x;
	FVector2D valuePivot = FVector2D(0.0f, 1.0f);
	FVector2D valueSize = FVector2D(250.0f, 30.0f);

	int itemInfoNameCount = mItemInfoNameText.size();
	int targetItemIndex = 0;
	auto itemInfo = CDataStorageManager::GetInst()->GetItemInfoDataByIndex(targetItemIndex);

	for (int dataIndex = 0, posIndex = 0; dataIndex < itemInfoNameCount; dataIndex++, posIndex++)
	{
		auto nameText = std::wstring(EItemInfoText::gArrItemInfoText[dataIndex]) + L":";
		mItemInfoNameText[dataIndex] = mScene->GetUIManager()->CreateWidget<CTextBlock>("itemInfoName_" + std::to_string(dataIndex));
		AddWidget(mItemInfoNameText[dataIndex]);
		mItemInfoNameText[dataIndex]->SetPivot(namePivot);
		mItemInfoNameText[dataIndex]->SetSize(nameSize);
		mItemInfoNameText[dataIndex]->SetPos(namePos - FVector2D::Axis[EAxis::Y] * nameSize.y * posIndex);
		mItemInfoNameText[dataIndex]->SetText(nameText.c_str());
		mItemInfoNameText[dataIndex]->SetTextColor(FVector4D::Green);
		mItemInfoNameText[dataIndex]->SetFontSize(nameSize.y - 10.0f);
		mItemInfoNameText[dataIndex]->SetAlignH(ETextAlignH::Left);
		mItemInfoNameText[dataIndex]->SetShadowEnable(false);
		mItemInfoNameText[dataIndex]->SetTextShadowColor(FVector4D::Gray30);
		mItemInfoNameText[dataIndex]->SetZOrder(ZORDER_LOBBY_TOOLTIP_TEXT);

		auto valueText = itemInfo.GetItemInfoWString(static_cast<EItemInfoText::Type>(dataIndex));
		mItemInfoValueText[dataIndex] = mScene->GetUIManager()->CreateWidget<CTextBlock>("itemInfoValue_" + std::to_string(dataIndex));
		AddWidget(mItemInfoValueText[dataIndex]);
		mItemInfoValueText[dataIndex]->SetPivot(valuePivot);

		if (dataIndex == EItemInfoText::Desc)
		{
			mItemInfoValueText[dataIndex]->SetSize(valueSize + FVector2D::Axis[EAxis::Y] * nameSize.y);
			mItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * (posIndex++));
		}
		else
		{
			mItemInfoValueText[dataIndex]->SetSize(valueSize);
			mItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * posIndex);
		}

		mItemInfoValueText[dataIndex]->SetText(valueText.c_str());
		mItemInfoValueText[dataIndex]->SetTextColor(FVector4D::Yellow);
		mItemInfoValueText[dataIndex]->SetFontSize(valueSize.y - 10.0f);
		mItemInfoValueText[dataIndex]->SetAlignH(ETextAlignH::Left);
		mItemInfoValueText[dataIndex]->SetShadowEnable(false);
		mItemInfoValueText[dataIndex]->SetTextShadowColor(FVector4D::Gray30);
		mItemInfoValueText[dataIndex]->SetZOrder(ZORDER_LOBBY_TOOLTIP_TEXT);
	}

	TriggerItemTooltip(-1);
}

void CLobbyWidget::InitDifficultiImage()
{
	// difficulty Image
	mMapDifficultyImage = mScene->GetUIManager()->CreateWidget<CImage>(mMapDifficultyImageNames[0]);
	AddWidget(mMapDifficultyImage);
	mMapDifficultyImage->SetTexture(mMapDifficultyImageNames[0]
		, mMapDifficultyImagePaths[0]);
	mMapDifficultyImage->SetPivot(FVector2D::One * 0.5f);
	mMapDifficultyImage->SetSize(FVector2D::One * 128 * 1.0f);
	mMapDifficultyImage->SetPos(mMapDifficultyImagePos);
	mMapDifficultyImage->SetColor(mMapDifficultyImageColors[curDifficultyIndex]);
}

void CLobbyWidget::InitNextPrevButton()
{
	// next button
	mNextButton = mScene->GetUIManager()->CreateWidget<CButton>("NextButton");
	AddWidget(mNextButton);
	SetButton(*mNextButton.Get(), "NextButton", ARROW_SQUARE_RIGHT_PATH);
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
	SetButton(*mPrevButton.Get(), "PrevButton", ARROW_SQUARE_LEFT_PATH);
	mPrevButton->SetPivot(FVector2D::One * 0.5f);
	mPrevButton->SetSize(FVector2D::One * 96.0f * 1.0f);
	mPrevButton->SetPos(FVector2D(100, 650));
	mPrevButton->SetEventCallback(EButtonEventState::Click
		, []()
		{
			CLog::PrintLog("mPrevButton Click");
			CSceneManager::GetInst()->CreateLoadScene<CSceneTitle>();
		});
}

void CLobbyWidget::InitItemButtons()
{
	// 슬롯은 이미지로 가자 -> 투명버튼 -> 이미지(CImage) -> 중간에 이미지 바꾸는거 됨.
	for (int i = 0; i < itemSlotCount; i++)
	{
		FVector2D tempPos = mSlotPosBase + mSlotPosAdd * i;
		// 슬롯에 '+' 있는것과 아무것도 없는 슬롯 스위칭 할것임.
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>(mSlotImageNames[(int)SlotType::ToAdd]);
		AddWidget(buttonBackImage);
		mItemSlotImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mSlotImageNames[(int)SlotType::ToAdd]
			, mSlotImagePaths[(int)SlotType::ToAdd]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize);
		buttonBackImage->SetColor(FVector4D::Green);
		buttonBackImage->SetPos(tempPos);

		// 슬롯 내부 아이템 이미지.
		CSharedPtr<CImage> buttonImage = mScene->GetUIManager()->CreateWidget<CImage>(mItemImageNames[i]);
		AddWidget(buttonImage);
		mItemImages.push_back(buttonImage);
		buttonImage->SetTexture(mItemImageNames[i], mItemImagePaths[i]);
		buttonImage->SetPivot(FVector2D::One * 0.5f);
		buttonImage->SetSize(mSlotSize * mSlotInnerItemSizeRate * mSlotInnerItemSizeRate);
		buttonImage->SetColor(FVector4D::Green);
		buttonImage->SetPos(tempPos);
		buttonImage->SetEnable(false);

		// 버튼이 인풋 이벤트에 입력되어야 하므로.
		CSharedPtr<CButton> slotButton = mScene->GetUIManager()->CreateWidget<CButton>("SlotButton");
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
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>(mSlotImageNames[(int)SlotType::Added]);
		AddWidget(buttonBackImage);
		mItemSlotImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mSlotImageNames[(int)SlotType::Added]
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
				//CLog::PrintLog("selectItemButton Click index: " + std::to_string(i));
				// 해당 아이템 선택했다는것.
				this->SelectItemForSlot(curSelectedSlot, i);
				// 아이템 배열 창 닫기.
				this->TriggerItemButtons(-1);
			});
		selectItemButton->SetEventCallback(EButtonEventState::Hovered
			, [this, i]()
			{
				//CLog::PrintLog("selectItemButton Hovered index: " + std::to_string(i));
				auto button = mItemButtons[i].Get();
				auto pos = button->GetPos() + FVector2D::Axis[EAxis::X] * button->GetSize().x * 0.8f;
				this->TriggerItemTooltip(i, pos);
			});
		selectItemButton->SetEventCallback(EButtonEventState::Unhoverd
			, [this, i]()
			{
				//CLog::PrintLog("selectItemButton Unhoverd index: " + std::to_string(i));
				this->TriggerItemTooltip(-1);
			});
	}
}

void CLobbyWidget::SelectItemForSlot(int _slotIndex, int _itemTypeIndex)
{
	// 슬롯 내 아이템 이미지가 변경이 되어야 함. 
	//CLog::PrintLog("SelectItemForSlot");
	//CLog::PrintLog("_slotIndex: " + std::to_string(_slotIndex));
	//CLog::PrintLog("_itemTypeIndex: " + std::to_string(_itemTypeIndex));
	
	CDataStorageManager::GetInst()->SetSelectedItemTypeInSlotIndex(_slotIndex, _itemTypeIndex);

	auto slotImage = mItemSlotImages[_slotIndex];
	slotImage->SetTexture(mSlotImageNames[(int)SlotType::Added]
		, mSlotImagePaths[(int)SlotType::Added]);

	auto itemImage = mItemImages[_slotIndex];
	itemImage->SetTexture(mItemImageNames[_itemTypeIndex]
		, mItemImagePaths[_itemTypeIndex]);
	itemImage->SetEnable(true);
}

void CLobbyWidget::TriggerItemButtons(int _index)
{
	// 슬롯위에 아이템 배열 창 나오게 하거나 / 사라지게 하는 기능.
	// _index 는 슬롯 위치를 위해 받는것.

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

void CLobbyWidget::TriggerItemTooltip(int _itemTypeIndex, FVector2D _pos)
{
	if (_itemTypeIndex == -1)
	{
		mToolTipShasdowImage->SetEnable(false);
		mToolTipBack2Image->SetEnable(false);
		mToolTipBackImage->SetEnable(false);

		int itemInfoNameCount = mItemInfoNameText.size();

		for (int dataIndex = 0; dataIndex < itemInfoNameCount; dataIndex++)
		{
			mItemInfoNameText[dataIndex].Get()->SetEnable(false);
			mItemInfoValueText[dataIndex].Get()->SetEnable(false);
		}

		return;
	}

	mToolTipShasdowImage->SetEnable(true);
	mToolTipBack2Image->SetEnable(true);
	mToolTipBackImage->SetEnable(true);

	// BackImages
	FVector2D backImagePos = _pos;
	FVector2D backImagePivot = FVector2D(0.0f, 0.5f);
	FVector2D backImageSize = FVector2D(340.0f, 155.0f);
	FVector2D back2ImagePos = backImagePos + FVector2D(-5.0f, 0.0f);
	FVector2D backShadowImagePos = back2ImagePos + FVector2D(10.0f, -10.0f);
	mToolTipShasdowImage->SetPos(backShadowImagePos);
	mToolTipBack2Image->SetPos(back2ImagePos);
	mToolTipBackImage->SetPos(backImagePos);

	// tooltip texts
	FVector2D namePos = backImagePos
		+ FVector2D(backImageSize.x * backImagePivot.x, backImageSize.y * backImagePivot.y)
		+ FVector2D(1.f, -1.f * 0.0f) * 5.0f;
	FVector2D namePivot = FVector2D(0.0f, 1.0f);
	FVector2D nameSize = FVector2D(100.0f, 30.0f);
	FVector2D valuePos = namePos + FVector2D::Axis[EAxis::X] * nameSize.x;
	FVector2D valuePivot = FVector2D(0.0f, 1.0f);
	FVector2D valueSize = FVector2D(250.0f, 30.0f);

	int itemInfoNameCount = mItemInfoNameText.size();
	auto itemInfo = CDataStorageManager::GetInst()->GetItemInfoDataByIndex(_itemTypeIndex);

	for (int dataIndex = 0, posIndex = 0; dataIndex < itemInfoNameCount; dataIndex++, posIndex++)
	{
		mItemInfoNameText[dataIndex].Get()->SetEnable(true);
		mItemInfoValueText[dataIndex].Get()->SetEnable(true);

		auto nameText = std::wstring(EItemInfoText::gArrItemInfoText[dataIndex]) + L":";
		mItemInfoNameText[dataIndex]->SetPos(namePos - FVector2D::Axis[EAxis::Y] * nameSize.y * posIndex);
		mItemInfoNameText[dataIndex]->SetText(nameText.c_str());

		auto valueText = itemInfo.GetItemInfoWString(static_cast<EItemInfoText::Type>(dataIndex));

		if (dataIndex == EItemInfoText::Desc)
			mItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * (posIndex++));
		else
			mItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * posIndex);

		mItemInfoValueText[dataIndex]->SetText(valueText.c_str());
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
	_button.SetPivot(FVector2D::One * 0.5f);
	_button.SetTint(EButtonState::Normal, TintNormal);
	_button.SetTint(EButtonState::Hovered, TintHovered);
	_button.SetTint(EButtonState::Click, TintClick);

}

void CLobbyWidget::OnCharacterLeftButtonClick()
{
	CLog::PrintLog("CharacterLeftButtonClick()");
	auto playerController = dynamic_cast<IScenePlayerGraphicController*>(mScene);

	if (playerController)
	{
		curPlayerGraphicIndex++;

		if (curPlayerGraphicIndex == CDataStorageManager::GetInst()->GetCharacterCount())
			curPlayerGraphicIndex = 0;

		playerController->SetChangeGraphic(0, curPlayerGraphicIndex);
		UpdatePlayerStatText();
	}
}

void CLobbyWidget::OnCharacterRightButtonClick()
{
	CLog::PrintLog("CharacterRightButtonClick()");
	auto playerController = dynamic_cast<IScenePlayerGraphicController*>(mScene);

	if (playerController)
	{
		if (curPlayerGraphicIndex > 0)
			curPlayerGraphicIndex--;
		else
			curPlayerGraphicIndex = CDataStorageManager::GetInst()->GetCharacterCount() - 1;

		playerController->SetChangeGraphic(0, curPlayerGraphicIndex);
		UpdatePlayerStatText();
	}
}

void CLobbyWidget::UpdatePlayerStatText()
{
	int count = (int)ECharacterStatText::End;
	FCharacterState stat = CDataStorageManager::GetInst()->GetCharacterState(curPlayerGraphicIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(ECharacterStatText::gArrCharacterStatText[i]) + L" :";
		auto valueText = stat.GetStatToWString(static_cast<ECharacterStatText::Type>(i));

		mPlayerStatNameText[i]->SetText(nameText.c_str());
		mPlayerStatValueText[i]->SetText(valueText.c_str());
	}
}

void CLobbyWidget::OnMapLeftButtonClick()
{
	CLog::PrintLog("MapLeftButtonClick()");

	curDifficultyIndex--;

	if (curDifficultyIndex < 0)
		curDifficultyIndex = CDataStorageManager::GetInst()->GetMapInfoCount() - 1;

	mMapDifficultyImage->SetTexture(mMapDifficultyImageNames[curDifficultyIndex]
		, mMapDifficultyImagePaths[curDifficultyIndex]);
	mMapDifficultyImage->SetColor(mMapDifficultyImageColors[curDifficultyIndex]);

	UpdateMapInfoText();
}

void CLobbyWidget::OnMapRightButtonClick()
{
	CLog::PrintLog("MapRightButtonClick()");

	curDifficultyIndex++;

	if (curDifficultyIndex == CDataStorageManager::GetInst()->GetMapInfoCount())
		curDifficultyIndex = 0;

	mMapDifficultyImage->SetTexture(mMapDifficultyImageNames[curDifficultyIndex]
		, mMapDifficultyImagePaths[curDifficultyIndex]);
	mMapDifficultyImage->SetColor(mMapDifficultyImageColors[curDifficultyIndex]);

	UpdateMapInfoText();
}

void CLobbyWidget::UpdateMapInfoText()
{
	int count = (int)EMapInfoText::End;
	FMapInfo info = CDataStorageManager::GetInst()->GetMapInfo(curDifficultyIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(EMapInfoText::gArrMapInfoText[i]) + L" :";
		auto valueText = info.GetInfoToWString(static_cast<EMapInfoText::Type>(i));

		mMapInfoNameText[i]->SetText(nameText.c_str());
		mMapInfoValueText[i]->SetText(valueText.c_str());
	}
}