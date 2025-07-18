﻿#include "LobbyWidget.h"
#include "Device.h"
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
#include "Etc/ZOrderContainer.h"
#include "Etc/NetworkManager.h"
#include "Etc/ProcessManager.h"
#include "Etc/MultiplayManager.h"
#include "Etc/TaskManager.h"

extern bool gIsOpenNicknamePopup;
CLobbyWidget::CLobbyWidget()
{
	mIsHost = false;
	mIsMultiPlay = false;
	/*mSlotTextureNamePrefix = "ButtonBackImage_";
	mItemTextureNamePrefix = "ButtonImage_";
	mSlotButtonNamePrefix = "SlotButton_";
	mMapDifficultyImageNamePrefix = "MapImage_";*/
	curSelectedSlot = ITEM_SLOT_DEFAULT_INDEX;

	curPlayerGraphicIndex = 0;
	curDifficultyIndex = 0;

	FResolution RS = CDevice::GetInst()->GetResolution();
	mResolution = FVector2D(RS.Width, RS.Height);

	for (int i = 0; i < CDataStorageManager::GetInst()->GetMapInfoCount(); i++)
	{
		auto mapInfo = CDataStorageManager::GetInst()->GetMapInfo(i);
		mArrMapDifficultyImagePath.push_back(FMapInfo::GetIconPath(i));
		mArrMapDifficultyImageName.push_back(FMapInfo::GetIconName(i));
		mArrMapDifficultyImageColor.push_back(FVector4D::GetColorFromString(mapInfo.ColorName));
	}

	{ // 아이템 정보 세팅.
		auto itemInfos = CDataStorageManager::GetInst()->GetItemInfoDatas();
		int index = 0;
		for (auto& item : itemInfos)
		{
			mArrItemImagePath.push_back(FItemInfo::GetItmeImagePath(index));
			mArrItemImageName.push_back(FItemInfo::GetItmeImageName(index));
			index++;
		}

		itemSlotCount = CDataStorageManager::GetInst()->GetSelectableItemCount();
		itemTypeCount = itemInfos.size();

		for (int i = 0; i < itemSlotCount; i++)
		{
			CDataStorageManager::GetInst()->SetSelectedItemTypeInSlotIndex(i, PLAYER_ITEM_TYPE_DEFAULT_INDEX);
		}
	}

	mArrSlotImagePath.push_back(ITEM_ADD_SQUARE_PATH);
	mArrSlotImagePath.push_back(ITEM_EMPTY_SQUARE_PATH);
	mArrSlotImagePath.push_back(ITEM_FULLY_SQUARE_PATH);
	mArrSlotImageName.push_back(ITEM_ADD_SQUARE_NAME);
	mArrSlotImageName.push_back(ITEM_EMPTY_SQUARE_NAME);
	mArrSlotImageName.push_back(ITEM_FULLY_SQUARE_NAME);

	mSlotPosBase = FVector2D(100, 100);
	mSlotPosAdd = FVector2D(105, 0);
	mSlotSize = FVector2D(100.0f, 100.0f);
	mSlotInnerItemSizeRate = 0.8f;
	mMapDifficultyImagePos = FVector2D(mResolution.x * 0.8f, mResolution.y * 0.4f);
	mMapDifficultySinAngle = 0.0f;

}

CLobbyWidget::~CLobbyWidget()
{
	RemoveListener();
}

bool CLobbyWidget::Init()
{
	CSceneWidget::Init();
	AddListener();

	CDataStorageManager::GetInst()->InitCurSelectedData();
	InitScrollSelectButtons();
	InitItemButtons();
	InitDifficultiImage();
	InitNextPrevButton();
	InitPlayerStatText();
	InitMapInfoText();
	InitItemInfoTooltip();

	if (CNetworkManager::GetInst()->IsMultiplay())
	{
		InitOtherPlayersInfo();
		UpdateOtherPlayerInfo();
		UpdateMapInfo();
	}
	else
	{
		InitSimpleCharacterRankInfo();
		InitSimpleMapRankInfo();
		UpdateSimpleCharacterRankInfo();
		UpdateSimpleMapRankInfo();
	}

	auto pathLeft = DIRECT_LEFT_PATH;
	auto pathRight = DIRECT_RIGHT_PATH;

	SetButton(*(mCharacterLeftButton.Get()), "CharacterLeftButton", pathLeft);
	SetButton(*(mCharacterRightButton.Get()), "CharacterRightButton", pathRight);
	SetButton(*(mMapLeftButton.Get()), "MapLeftButton", pathLeft);
	SetButton(*(mMapRightButton.Get()), "MapRightButton", pathRight);

	mArrItemStatAddValue.resize(ECharacterStatText::End, 0.0f);

	return true;
}

void CLobbyWidget::Update(float DeltaTime)
{
	CSceneWidget::Update(DeltaTime);

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

	if (CNetworkManager::GetInst()->IsMultiplay())
	{
		bool _isHost = CMultiplayManager::GetInst()->GetIsHost();
		mMapLeftButton->SetEnable(_isHost);
		mMapRightButton->SetEnable(_isHost);
	}
	else
	{
		mMapLeftButton->SetEnable(true);
		mMapRightButton->SetEnable(true);
	}

}

void CLobbyWidget::InitPlayerStatText()
{
	int count = (int)ECharacterStatText::End;
	mArrPlayerStatNameText.resize(count);
	mArrPlayerStatValueText.resize(count);

	FVector2D textSize = FVector2D(200.0f, 40.0f);

	FVector2D nameBasePivot = FVector2D(0.0f, 0.0f);
	FVector2D nameBasePos = FVector2D(550.0f, mResolution.y * 0.87f);

	FVector2D playerStatValueBasePivot = FVector2D(0.0f, 0.0f);
	FVector2D playerStatValueBasePos = FVector2D(700.0f, mResolution.y * 0.87f);

	float fontSize = 30.0f;

	FCharacterState stat = CDataStorageManager::GetInst()->GetCharacterState(curPlayerGraphicIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(ECharacterStatText::gArrCharacterStatText[i]) + L" :";
		auto valueText = stat.GetStatToWString(static_cast<ECharacterStatText::Type>(i));
		auto textBlockName = mScene->GetUIManager()->CreateWidget<CTextBlock>("playerStatName_" + std::to_string(i));
		auto textBlockPlayerStatValue = mScene->GetUIManager()->CreateWidget<CTextBlock>("playerStatValue_" + std::to_string(i));

		AddWidget(textBlockName);
		textBlockName->SetPivot(nameBasePivot);
		textBlockName->SetSize(textSize);
		textBlockName->SetPos(nameBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 10.0f) * i);
		textBlockName->SetText(nameText.c_str());
		textBlockName->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		textBlockName->SetAlignH(ETextAlignH::Left);
		textBlockName->SetFontSize(fontSize);
		textBlockName->SetShadowEnable(true);
		textBlockName->SetShadowOffset(2.f, 2.f);
		textBlockName->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mArrPlayerStatNameText[i] = textBlockName;

		AddWidget(textBlockPlayerStatValue);
		textBlockPlayerStatValue->SetPivot(playerStatValueBasePivot);
		textBlockPlayerStatValue->SetSize(textSize);
		textBlockPlayerStatValue->SetPos(playerStatValueBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 10.0f) * i);
		textBlockPlayerStatValue->SetText(valueText.c_str());
		textBlockPlayerStatValue->SetTextColor(FVector4D::GetColorFromString(stat.ColorName));
		textBlockPlayerStatValue->SetAlignH(ETextAlignH::Left);
		textBlockPlayerStatValue->SetFontSize(fontSize);
		textBlockPlayerStatValue->SetShadowEnable(true);
		textBlockPlayerStatValue->SetShadowOffset(2.f, 2.f);
		textBlockPlayerStatValue->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mArrPlayerStatValueText[i] = textBlockPlayerStatValue;

	}
}

void CLobbyWidget::InitMapInfoText()
{
	int count = (int)EMapInfoText::End;
	mArrMapInfoNameText.resize(count);
	mArrMapInfoValueText.resize(count);

	FVector2D nameBasePivot = FVector2D(0.0f, 0.0f);
	FVector2D nameBasePos = FVector2D(550.0f, mResolution.y * 0.4f);

	FVector2D valueBasePivot = FVector2D(0.0f, 0.0f);
	FVector2D valueBasePos = FVector2D(700.0f, mResolution.y * 0.4f);

	FVector2D textSize = FVector2D(200.0f, 100.0f);
	float fontSize = 30.0f;
	FMapInfo info = CDataStorageManager::GetInst()->GetMapInfo(curDifficultyIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(EMapInfoText::gArrMapInfoText[i]) + L" :";
		auto valueText = info.GetInfoToWString(static_cast<EMapInfoText::Type>(i));
		auto textBlockName = mScene->GetUIManager()->CreateWidget<CTextBlock>("mapInfoName_" + std::to_string(i));
		auto textBlockPlayerStatValue = mScene->GetUIManager()->CreateWidget<CTextBlock>("mapInfoValue_" + std::to_string(i));

		AddWidget(textBlockName);
		textBlockName->SetPivot(nameBasePivot);
		textBlockName->SetSize(textSize);
		textBlockName->SetPos(nameBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 15.0f) * i);
		textBlockName->SetText(nameText.c_str());
		textBlockName->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		textBlockName->SetAlignH(ETextAlignH::Left);
		textBlockName->SetFontSize(fontSize);
		textBlockName->SetShadowEnable(true);
		textBlockName->SetShadowOffset(2.f, 2.f);
		textBlockName->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mArrMapInfoNameText[i] = textBlockName;

		AddWidget(textBlockPlayerStatValue);
		textBlockPlayerStatValue->SetPivot(valueBasePivot);
		textBlockPlayerStatValue->SetSize(textSize);
		textBlockPlayerStatValue->SetPos(valueBasePos - FVector2D::Axis[EAxis::Y] * (fontSize + 15.0f) * i);
		textBlockPlayerStatValue->SetText(valueText.c_str());
		textBlockPlayerStatValue->SetTextColor(FVector4D::GetColorFromString(info.ColorName));
		textBlockPlayerStatValue->SetAlignH(ETextAlignH::Left);
		textBlockPlayerStatValue->SetFontSize(fontSize);
		textBlockPlayerStatValue->SetShadowEnable(true);
		textBlockPlayerStatValue->SetShadowOffset(2.f, 2.f);
		textBlockPlayerStatValue->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mArrMapInfoValueText[i] = textBlockPlayerStatValue;
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
	mToolTipShasdowImage->SetColor(FVector4D::GetColorFromString(COLOR_DarkGray));
	mToolTipShasdowImage->SetPos(backShadowImagePos);
	mToolTipShasdowImage->SetZOrder(ZORDER_LOBBY_TOOLTIP_SHADOW);

	mToolTipBack2Image = mScene->GetUIManager()->CreateWidget<CImage>("mToolTipBack2Image");
	AddWidget(mToolTipBack2Image);
	mToolTipBack2Image->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mToolTipBack2Image->SetPivot(backImagePivot);
	mToolTipBack2Image->SetSize(back2ImageSize);
	mToolTipBack2Image->SetColor(FVector4D::GetColorFromString(COLOR_Green));
	mToolTipBack2Image->SetPos(back2ImagePos);
	mToolTipBack2Image->SetZOrder(ZORDER_LOBBY_TOOLTIP_BACK);

	mToolTipBackImage = mScene->GetUIManager()->CreateWidget<CImage>("mToolTipBackImage");
	AddWidget(mToolTipBackImage);
	mToolTipBackImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mToolTipBackImage->SetPivot(backImagePivot);
	mToolTipBackImage->SetSize(backImageSize);
	mToolTipBackImage->SetColor(FVector4D::GetColorFromString(COLOR_Black) * 0.9f);
	mToolTipBackImage->SetPos(backImagePos);
	mToolTipBackImage->SetZOrder(ZORDER_LOBBY_TOOLTIP);

	mArrItemInfoNameText.resize(EItemInfoText::End);
	mArrItemInfoValueText.resize(EItemInfoText::End);

	FVector2D namePos = backImagePos
		+ FVector2D(backImageSize.x * backImagePivot.x, backImageSize.y * backImagePivot.y)
		+ FVector2D(1.f, -1.f * 0.0f) * 5.0f;
	FVector2D namePivot = FVector2D(0.0f, 1.0f);
	FVector2D nameSize = FVector2D(100.0f, 30.0f);
	FVector2D valuePos = namePos + FVector2D::Axis[EAxis::X] * nameSize.x;
	FVector2D valuePivot = FVector2D(0.0f, 1.0f);
	FVector2D valueSize = FVector2D(250.0f, 30.0f);

	int itemInfoNameCount = mArrItemInfoNameText.size();
	int targetItemIndex = 0;
	auto itemInfo = CDataStorageManager::GetInst()->GetItemInfoDataByIndex(targetItemIndex);

	for (int dataIndex = 0, posIndex = 0; dataIndex < itemInfoNameCount; dataIndex++, posIndex++)
	{
		auto nameText = std::wstring(EItemInfoText::gArrItemInfoText[dataIndex]) + L":";
		mArrItemInfoNameText[dataIndex] = mScene->GetUIManager()->CreateWidget<CTextBlock>("itemInfoName_" + std::to_string(dataIndex));
		AddWidget(mArrItemInfoNameText[dataIndex]);
		mArrItemInfoNameText[dataIndex]->SetPivot(namePivot);
		mArrItemInfoNameText[dataIndex]->SetSize(nameSize);
		mArrItemInfoNameText[dataIndex]->SetPos(namePos - FVector2D::Axis[EAxis::Y] * nameSize.y * posIndex);
		mArrItemInfoNameText[dataIndex]->SetText(nameText.c_str());
		mArrItemInfoNameText[dataIndex]->SetTextColor(FVector4D::GetColorFromString(COLOR_White));
		mArrItemInfoNameText[dataIndex]->SetFontSize(nameSize.y - 10.0f);
		mArrItemInfoNameText[dataIndex]->SetAlignH(ETextAlignH::Left);
		mArrItemInfoNameText[dataIndex]->SetShadowEnable(false);
		mArrItemInfoNameText[dataIndex]->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mArrItemInfoNameText[dataIndex]->SetZOrder(ZORDER_LOBBY_TOOLTIP_TEXT);

		auto valueText = itemInfo.GetItemInfoWString(static_cast<EItemInfoText::Type>(dataIndex));
		mArrItemInfoValueText[dataIndex] = mScene->GetUIManager()->CreateWidget<CTextBlock>("itemInfoValue_" + std::to_string(dataIndex));
		AddWidget(mArrItemInfoValueText[dataIndex]);
		mArrItemInfoValueText[dataIndex]->SetPivot(valuePivot);

		if (dataIndex == EItemInfoText::Desc)
		{
			mArrItemInfoValueText[dataIndex]->SetSize(valueSize + FVector2D::Axis[EAxis::Y] * nameSize.y);
			mArrItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * (posIndex++));
		}
		else
		{
			mArrItemInfoValueText[dataIndex]->SetSize(valueSize);
			mArrItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * posIndex);
		}

		mArrItemInfoValueText[dataIndex]->SetText(valueText.c_str());
		mArrItemInfoValueText[dataIndex]->SetTextColor(FVector4D::GetColorFromString(COLOR_Yellow));
		mArrItemInfoValueText[dataIndex]->SetFontSize(valueSize.y - 10.0f);
		mArrItemInfoValueText[dataIndex]->SetAlignH(ETextAlignH::Left);
		mArrItemInfoValueText[dataIndex]->SetShadowEnable(false);
		mArrItemInfoValueText[dataIndex]->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mArrItemInfoValueText[dataIndex]->SetZOrder(ZORDER_LOBBY_TOOLTIP_TEXT);
	}

	TriggerItemTooltip(PLAYER_ITEM_TYPE_DEFAULT_INDEX);
}

void CLobbyWidget::InitDifficultiImage()
{
	if (CNetworkManager::GetInst()->IsMultiplay())
		curDifficultyIndex = CMultiplayManager::GetInst()->GetCurMapIndex();

	// difficulty Image
	mMapDifficultyImage = mScene->GetUIManager()->CreateWidget<CImage>(mArrMapDifficultyImageName[0]);
	AddWidget(mMapDifficultyImage);
	mMapDifficultyImage->SetTexture(mArrMapDifficultyImageName[0]
		, mArrMapDifficultyImagePath[0]);
	mMapDifficultyImage->SetPivot(FVector2D::One * 0.5f);
	mMapDifficultyImage->SetSize(FVector2D::One * 128 * 1.0f);
	mMapDifficultyImage->SetPos(mMapDifficultyImagePos);
	mMapDifficultyImage->SetColor(mArrMapDifficultyImageColor[curDifficultyIndex]);
}

void CLobbyWidget::InitNextPrevButton()
{
	// next button
	mNextButton = mScene->GetUIManager()->CreateWidget<CButton>(ARROW_SQUARE_RIGHT_NAME);
	AddWidget(mNextButton);
	SetButton(*mNextButton.Get(), ARROW_SQUARE_RIGHT_NAME, ARROW_SQUARE_RIGHT_PATH);
	mNextButton->SetPivot(FVector2D::One * 0.5f);
	mNextButton->SetSize(FVector2D::One * 128 * 1.0f);
	mNextButton->SetPos(FVector2D(1150.0f, 100.0f));
	mNextButton->SetEventCallback(EButtonEventState::Click
		, [this]()
		{
			if (CNetworkManager::GetInst()->IsMultiplay())
			{
				if (CMultiplayManager::GetInst()->GetIsHost())
					SendMessageTrigger(ClientMessage::MSG_START);
				else
				{
					if (CMultiplayManager::GetInst()->GetPlayerInfoFromMyId().isReady)
						SendMessageTrigger(ClientMessage::MSG_UNREADY);
					else
						SendMessageTrigger(ClientMessage::MSG_READY);
					return;
				}
			}
			else
			{
				StartGame();
			}
		});

	// previous button
	mPrevButton = mScene->GetUIManager()->CreateWidget<CButton>(ARROW_SQUARE_LEFT_NAME);
	AddWidget(mPrevButton);
	SetButton(*mPrevButton.Get(), ARROW_SQUARE_LEFT_NAME, ARROW_SQUARE_LEFT_PATH);
	mPrevButton->SetPivot(FVector2D::One * 0.5f);
	mPrevButton->SetSize(FVector2D::One * 96.0f * 1.0f);
	mPrevButton->SetPos(FVector2D(100.0f, 650.0f));
	mPrevButton->SetEventCallback(EButtonEventState::Click
		, [this]()
		{
			if (gIsOpenNicknamePopup)
				return;

			CLog::PrintLog("mPrevButton Click");
			LoadScene(EGameScene::Title);
		});
}

void CLobbyWidget::InitItemButtons()
{
	// 슬롯은 이미지로 가자 -> 투명버튼 -> 이미지(CImage) -> 중간에 이미지 바꾸는거 됨.
	for (int i = 0; i < itemSlotCount; i++)
	{
		FVector2D tempPos = mSlotPosBase + mSlotPosAdd * i;
		// 슬롯에 '+' 있는것과 아무것도 없는 슬롯 스위칭 할것임.
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>(mArrSlotImageName[(int)SlotType::ToAdd]);
		AddWidget(buttonBackImage);
		mArrItemSlotImage.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mArrSlotImageName[(int)SlotType::ToAdd]
			, mArrSlotImagePath[(int)SlotType::ToAdd]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize);
		buttonBackImage->SetColor(FVector4D::GetColorFromString(COLOR_Green));
		buttonBackImage->SetPos(tempPos);
		buttonBackImage->SetZOrder(ZORDER_LOBBY_MY_ITEM_SLOT);

		// 슬롯 내부 아이템 이미지.
		CSharedPtr<CImage> buttonImage = mScene->GetUIManager()->CreateWidget<CImage>(mArrItemImageName[i]);
		AddWidget(buttonImage);
		mArrItemImageInSlot.push_back(buttonImage);
		buttonImage->SetTexture(mArrItemImageName[i], mArrItemImagePath[i]);
		buttonImage->SetPivot(FVector2D::One * 0.5f);
		buttonImage->SetSize(mSlotSize * mSlotInnerItemSizeRate * mSlotInnerItemSizeRate);
		buttonImage->SetColor(FVector4D::GetColorFromString(COLOR_Green));
		buttonImage->SetPos(tempPos);
		buttonImage->SetZOrder(ZORDER_LOBBY_MY_ITEM_ICON);
		buttonImage->SetEnable(false);

		// 버튼이 인풋 이벤트에 입력되어야 하므로.
		CSharedPtr<CButton> slotButton = mScene->GetUIManager()->CreateWidget<CButton>("SlotButton");
		AddWidget(slotButton);
		mArrItemSlotButton.push_back(slotButton);
		slotButton->SetSize(mSlotSize);
		slotButton->SetPivot(FVector2D::One * 0.5f);
		slotButton->SetColor(FVector4D::GetColorFromString(COLOR_Transparent));
		slotButton->SetPos(tempPos);
		slotButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		slotButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("slotButton index: " + std::to_string(i));
				this->TriggerItemArrayButtons(i);
			});
	}

	for (int i = 0; i < itemTypeCount; i++)
	{
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>(mArrSlotImageName[(int)SlotType::Fully]);
		AddWidget(buttonBackImage);
		buttonBackImage->SetTexture(mArrSlotImageName[(int)SlotType::Fully]
			, mArrSlotImagePath[(int)SlotType::Fully]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize * pow(mSlotInnerItemSizeRate, 1));
		buttonBackImage->SetColor(FVector4D::GetColorFromString(COLOR_Black));
		buttonBackImage->SetZOrder(ZORDER_LOBBY_MY_ITEM_SLOT);
		buttonBackImage->SetEnable(false);
		mArrSlotImageInList.push_back(buttonBackImage);

		CSharedPtr<CImage> buttonFrameImage = mScene->GetUIManager()->CreateWidget<CImage>(mArrSlotImageName[(int)SlotType::Added]);
		AddWidget(buttonFrameImage);
		buttonFrameImage->SetTexture(mArrSlotImageName[(int)SlotType::Added]
			, mArrSlotImagePath[(int)SlotType::Added]);
		buttonFrameImage->SetPivot(FVector2D::One * 0.5f);
		buttonFrameImage->SetSize(mSlotSize * pow(mSlotInnerItemSizeRate, 1));
		buttonFrameImage->SetColor(FVector4D::GetColorFromString(COLOR_Green));
		buttonFrameImage->SetZOrder(ZORDER_LOBBY_MY_ITEM_ICON);
		buttonFrameImage->SetEnable(false);
		mArrSlotFrameImageInList.push_back(buttonFrameImage);

		std::string name = "SelectItemButton_" + std::to_string(i);
		CSharedPtr<CButton> selectItemButton = mScene->GetUIManager()->CreateWidget<CButton>(name);
		AddWidget(selectItemButton);
		mArrItemButtonInList.push_back(selectItemButton);
		this->SetButton(*selectItemButton.Get(), name.c_str(), mArrItemImagePath[i]);

		selectItemButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		selectItemButton->SetPivot(FVector2D::One * 0.5f);
		selectItemButton->SetSize(mSlotSize * pow(mSlotInnerItemSizeRate, 3));
		selectItemButton->SetEnable(false);
		selectItemButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				//CLog::PrintLog("selectItemButton Click index: " + std::to_string(i));
				// 해당 아이템 선택했다는것.
				this->SelectItemForSlot(curSelectedSlot, i);
				this->UpdatePlayerStat();

				// 아이템 배열 창 닫기.
				this->TriggerItemArrayButtons(ITEM_SLOT_DEFAULT_INDEX);
			});
		selectItemButton->SetEventCallback(EButtonEventState::Hovered
			, [this, i]()
			{
				//CLog::PrintLog("selectItemButton Hovered index: " + std::to_string(i));
				auto button = mArrItemButtonInList[i].Get();
				auto pos = button->GetPos() + FVector2D::Axis[EAxis::X] * button->GetSize().x * 0.8f;
				this->TriggerItemTooltip(i, pos);
			});
		selectItemButton->SetEventCallback(EButtonEventState::Unhoverd
			, [this, i]()
			{
				//CLog::PrintLog("selectItemButton Unhoverd index: " + std::to_string(i));
				this->TriggerItemTooltip(PLAYER_ITEM_TYPE_DEFAULT_INDEX);
			});
	}
}

void CLobbyWidget::SelectItemForSlot(int _slotIndex, int _itemTypeIndex)
{
	// 슬롯 내 아이템 이미지가 변경이 되어야 함. 
	//CLog::PrintLog("SelectItemForSlot");
	//CLog::PrintLog("_slotIndex: " + std::to_string(_slotIndex));
	//CLog::PrintLog("_itemTypeIndex: " + std::to_string(_itemTypeIndex));

	// 현재 내가 선택한 아이템 세팅.
	CDataStorageManager::GetInst()->SetSelectedItemTypeInSlotIndex(_slotIndex, _itemTypeIndex);

	auto slotImage = mArrItemSlotImage[_slotIndex];
	slotImage->SetTexture(mArrSlotImageName[(int)SlotType::Added]
		, mArrSlotImagePath[(int)SlotType::Added]);

	auto itemImage = mArrItemImageInSlot[_slotIndex];
	itemImage->SetTexture(mArrItemImageName[_itemTypeIndex]
		, mArrItemImagePath[_itemTypeIndex]);
	itemImage->SetEnable(true);

	for (float& e : mArrItemStatAddValue)
		e = 0.0f;


	for (int i = 0; i < itemSlotCount; i++)
	{
		auto _selectedItemIndex = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(i);

		if (_selectedItemIndex < 0)
			continue;

		auto _itemStatInfo = CDataStorageManager::GetInst()->GetItemInfoDataByIndex(_selectedItemIndex);

		int _indexPlayerStatFromCommonStat = 0;

		switch ((EStatInfoText::Type)_itemStatInfo.StatType)
		{
		case EStatInfoText::HP:
			_indexPlayerStatFromCommonStat = (int)ECharacterStatText::HP;
			break;
		case EStatInfoText::Speed:
			_indexPlayerStatFromCommonStat = (int)ECharacterStatText::Speed;
			break;
		case EStatInfoText::Dex:
			_indexPlayerStatFromCommonStat = (int)ECharacterStatText::Dex;
			break;
		case EStatInfoText::Def:
			_indexPlayerStatFromCommonStat = (int)ECharacterStatText::Def;
			break;
		}

		mArrItemStatAddValue[_indexPlayerStatFromCommonStat] += _itemStatInfo.AddValue;
	}

	if (CNetworkManager::GetInst()->IsMultiplay())
	{
		SendMessageTriggerItem(ClientMessage::MSG_PICK_ITEM, _slotIndex, _itemTypeIndex);
	}
}

void CLobbyWidget::TriggerItemArrayButtons(int _itemSlotIndex)
{
	// 슬롯위에 아이템 배열 창 나오게 하거나 / 사라지게 하는 기능.
	// _itemSlotIndex 는 슬롯 위치를 위해 받는것.

	curSelectedSlot = _itemSlotIndex;

	if (_itemSlotIndex == ITEM_SLOT_DEFAULT_INDEX)
	{
		for (int i = 0; i < itemTypeCount; i++)
		{
			mArrItemButtonInList[i]->SetEnable(false);
			mArrSlotImageInList[i]->SetEnable(false);
			mArrSlotFrameImageInList[i]->SetEnable(false);
		}
		return;
	}

	// 인덱스 들어온 지점에, 순서대로 세로 위로 쪼르륵 아이템 나열 -> 선택 가능하게끔.
	for (int i = 0; i < itemTypeCount; i++)
	{
		FVector2D tempPos = mSlotPosBase + FVector2D::Axis[EAxis::Y] * 10 + mSlotPosAdd * _itemSlotIndex // 가로 슬롯 기준 위치 -> 몇번쨰 슬롯에서 눌렸지?
			+ FVector2D(0, mSlotSize.y * (i + 1)) * mSlotInnerItemSizeRate // 세로로 한칸씩 올릴꺼
			+ FVector2D(0, 5) * (i + 1); // 간격
		mArrItemButtonInList[i]->SetEnable(true);
		mArrItemButtonInList[i]->SetPos(tempPos);
		mArrSlotImageInList[i]->SetEnable(true);
		mArrSlotImageInList[i]->SetPos(tempPos);
		mArrSlotFrameImageInList[i]->SetEnable(true);
		mArrSlotFrameImageInList[i]->SetPos(tempPos);
	}
}

void CLobbyWidget::TriggerItemTooltip(int _itemTypeIndex, FVector2D _pos)
{
	if (_itemTypeIndex == PLAYER_ITEM_TYPE_DEFAULT_INDEX)
	{
		mToolTipShasdowImage->SetEnable(false);
		mToolTipBack2Image->SetEnable(false);
		mToolTipBackImage->SetEnable(false);

		int itemInfoNameCount = mArrItemInfoNameText.size();

		for (int dataIndex = 0; dataIndex < itemInfoNameCount; dataIndex++)
		{
			mArrItemInfoNameText[dataIndex].Get()->SetEnable(false);
			mArrItemInfoValueText[dataIndex].Get()->SetEnable(false);
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

	int itemInfoNameCount = mArrItemInfoNameText.size();
	auto itemInfo = CDataStorageManager::GetInst()->GetItemInfoDataByIndex(_itemTypeIndex);

	for (int dataIndex = 0, posIndex = 0; dataIndex < itemInfoNameCount; dataIndex++, posIndex++)
	{
		mArrItemInfoNameText[dataIndex].Get()->SetEnable(true);
		mArrItemInfoValueText[dataIndex].Get()->SetEnable(true);

		auto nameText = std::wstring(EItemInfoText::gArrItemInfoText[dataIndex]) + L":";
		mArrItemInfoNameText[dataIndex]->SetPos(namePos - FVector2D::Axis[EAxis::Y] * nameSize.y * posIndex);
		mArrItemInfoNameText[dataIndex]->SetText(nameText.c_str());

		auto valueText = itemInfo.GetItemInfoWString(static_cast<EItemInfoText::Type>(dataIndex));

		if (dataIndex == EItemInfoText::Desc)
			mArrItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * (posIndex++));
		else
			mArrItemInfoValueText[dataIndex]->SetPos(valuePos - FVector2D::Axis[EAxis::Y] * valueSize.y * posIndex);

		mArrItemInfoValueText[dataIndex]->SetText(valueText.c_str());
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

void CLobbyWidget::StartGame()
{
	CLog::PrintLog("mNextButton Click curPlayerGraphicIndex: " + std::to_string(curPlayerGraphicIndex));
	CLog::PrintLog("mNextButton Click curDifficultyIndex: " + std::to_string(curDifficultyIndex));
	LoadScene(EGameScene::InGame);
}

void CLobbyWidget::UpdateOtherPlayerInfo()
{
	// init
	for (int i = 0; i < mArrPlayerWidgetGroup.size(); i++)
	{
		// player check back
		mArrPlayerWidgetGroup[i].mPlayerCheckBackImage->SetEnable(false);

		// host init
		mArrPlayerWidgetGroup[i].mPlayerHostImage->SetEnable(false);

		// player text init 
		mArrPlayerWidgetGroup[i].mPlayerText->SetText(PLAYER_EMPTY_TEXT);

		// character color init 
		auto color = FVector4D::GetColorFromString(COLOR_White);

		// ready init 
		color.w = 0.5f;
		mArrPlayerWidgetGroup[i].mPlayerText->SetTextColor(color);

		// item init 
		for (int j = 0; j < mArrPlayerWidgetGroup[i].mArrPlayerItemImage.size(); j++)
		{
			auto _image = mArrPlayerWidgetGroup[i].mArrPlayerItemImage[j];
			auto _slotImage = mArrPlayerWidgetGroup[i].mArrPlayerSlotImage[j];
			_image->SetEnable(false);

			_image->SetColor(color);
			_slotImage->SetColor(color);
		}

	}

	// setting from multiplay info data
	int count = CMultiplayManager::GetInst()->GetPlayerCount();
	int myId = CMultiplayManager::GetInst()->GetMyId();

	for (int i = 0; i < count; i++)
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoByIndex(i);
		// player check back
		mArrPlayerWidgetGroup[i].mPlayerCheckBackImage->SetEnable(info.id == myId);

		// host
		mArrPlayerWidgetGroup[i].mPlayerHostImage->SetEnable(info.isHost);

		// player text
		std::wstring _nickname;
		ConvertWStringAndCheckTextOverLength(info.nickname, _nickname, PLAYER_LOBBY_NICKNAME_LENGTH_MAX_SCORE);
		mArrPlayerWidgetGroup[i].mPlayerText->SetText(_nickname.c_str());

		// character color
		auto characterInfo = CDataStorageManager::GetInst()->GetCharacterState(info.characterType);
		auto color = FVector4D::GetColorFromString(characterInfo.ColorName);

		// ready
		color.w = info.isReady || info.isHost ? 1.0f : 0.5f;
		mArrPlayerWidgetGroup[i].mPlayerText->SetTextColor(color);

		// item
		for (int j = 0; j < info.arrItemType.size(); j++)
		{
			bool _isEnable = info.arrItemType[j] != PLAYER_ITEM_TYPE_DEFAULT_INDEX;
			auto _image = mArrPlayerWidgetGroup[i].mArrPlayerItemImage[j];
			auto _slotImage = mArrPlayerWidgetGroup[i].mArrPlayerSlotImage[j];

			_image->SetEnable(_isEnable);

			if (_isEnable)
				_image->SetTexture(mArrItemImageName[info.arrItemType[j]], mArrItemImagePath[info.arrItemType[j]]);

			_image->SetColor(color);
			_slotImage->SetColor(color);
		}

	}
}

void CLobbyWidget::UpdateSimpleCharacterRankInfo()
{
	// init
	for (int i = 0; i < mArrRankWidgetGroupByCharacter.size(); i++)
	{
		// player text init 
		mArrRankWidgetGroupByCharacter[i].mPlayerText->SetText(PLAYER_EMPTY_TEXT);
		mArrRankWidgetGroupByCharacter[i].mPlayerText->SetEnable(false);

		// distance text init
		mArrRankWidgetGroupByCharacter[i].mDistanceText->SetText((FormatWithCommaManual(12345) + L"m").c_str());
		mArrRankWidgetGroupByCharacter[i].mDistanceText->SetEnable(false);

		// character color init 
		auto color = FVector4D::GetColorFromString(COLOR_White);

		// ready init 
		mArrRankWidgetGroupByCharacter[i].mPlayerText->SetTextColor(color);

		// item init 
		for (int j = 0; j < mArrRankWidgetGroupByCharacter[i].mArrPlayerItemImage.size(); j++)
		{
			auto _image = mArrRankWidgetGroupByCharacter[i].mArrPlayerItemImage[j];
			auto _slotImage = mArrRankWidgetGroupByCharacter[i].mArrPlayerSlotImage[j];
			_image->SetEnable(false);
			_slotImage->SetEnable(false);

			_image->SetColor(color);
			_slotImage->SetColor(color);
		}

	}

	auto _arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(ERankMenuTap::Character, curPlayerGraphicIndex);
	int _minCount = PLAYER_SIMPLE_RANK > _arrRankInfo.size() ? _arrRankInfo.size() : PLAYER_SIMPLE_RANK;

	for (int i = 0; i < _minCount; i++)
	{
		auto _info = _arrRankInfo[i];
		auto mapInfo = CDataStorageManager::GetInst()->GetMapInfo(_info.Map);
		auto color = FVector4D::GetColorFromString(mapInfo.ColorName);

		std::wstring _nickname;
		ConvertWStringAndCheckTextOverLength(_info.Name, _nickname, PLAYER_LOBBY_NICKNAME_LENGTH_MAX_SCORE);
		mArrRankWidgetGroupByCharacter[i].mPlayerText->SetText(_nickname.c_str());
		mArrRankWidgetGroupByCharacter[i].mPlayerText->SetTextColor(color);
		mArrRankWidgetGroupByCharacter[i].mPlayerText->SetEnable(true);
		mArrRankWidgetGroupByCharacter[i].mDistanceText->SetText((FormatWithCommaManual(_info.Distance) + L"m").c_str());
		mArrRankWidgetGroupByCharacter[i].mDistanceText->SetTextColor(color);
		mArrRankWidgetGroupByCharacter[i].mDistanceText->SetEnable(true);

		std::vector<int> _arrItemType = { _info.Item_0, _info.Item_1, _info.Item_2 };

		for (int j = 0; j < _arrItemType.size(); j++)
		{
			int _itemType = _arrItemType[j];
			bool _isEnable = _itemType != PLAYER_ITEM_TYPE_DEFAULT_INDEX;
			auto _image = mArrRankWidgetGroupByCharacter[i].mArrPlayerItemImage[j];
			auto _slotImage = mArrRankWidgetGroupByCharacter[i].mArrPlayerSlotImage[j];

			_slotImage->SetEnable(true);
			_image->SetEnable(_isEnable);

			if (_isEnable)
				_image->SetTexture(mArrItemImageName[_itemType], mArrItemImagePath[_itemType]);

			_image->SetColor(color);
			_slotImage->SetColor(color);
		}
	}
}

void CLobbyWidget::UpdateSimpleMapRankInfo()
{
	// init
	for (int i = 0; i < mArrRankWidgetGroupByMap.size(); i++)
	{
		// player text init 
		mArrRankWidgetGroupByMap[i].mPlayerText->SetText(PLAYER_EMPTY_TEXT);
		mArrRankWidgetGroupByMap[i].mPlayerText->SetEnable(false);

		// distance text init
		mArrRankWidgetGroupByMap[i].mDistanceText->SetText((FormatWithCommaManual(12345) + L"m").c_str());
		mArrRankWidgetGroupByMap[i].mDistanceText->SetEnable(false);

		// character color init 
		auto color = FVector4D::GetColorFromString(COLOR_White);

		// ready init 
		mArrRankWidgetGroupByMap[i].mPlayerText->SetTextColor(color);

		// item init 
		for (int j = 0; j < mArrRankWidgetGroupByMap[i].mArrPlayerItemImage.size(); j++)
		{
			auto _image = mArrRankWidgetGroupByMap[i].mArrPlayerItemImage[j];
			auto _slotImage = mArrRankWidgetGroupByMap[i].mArrPlayerSlotImage[j];
			_image->SetEnable(false);
			_slotImage->SetEnable(false);

			_image->SetColor(color);
			_slotImage->SetColor(color);
		}

	}

	auto _arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(ERankMenuTap::Map, curDifficultyIndex);
	int _minCount = PLAYER_SIMPLE_RANK > _arrRankInfo.size() ? _arrRankInfo.size() : PLAYER_SIMPLE_RANK;

	for (int i = 0; i < _minCount; i++)
	{
		auto _info = _arrRankInfo[i];
		auto characterInfo = CDataStorageManager::GetInst()->GetCharacterState(_info.Character);
		auto color = FVector4D::GetColorFromString(characterInfo.ColorName);

		std::wstring _nickname;
		ConvertWStringAndCheckTextOverLength(_info.Name, _nickname, PLAYER_LOBBY_NICKNAME_LENGTH_MAX_SCORE);
		mArrRankWidgetGroupByMap[i].mPlayerText->SetText(_nickname.c_str());
		mArrRankWidgetGroupByMap[i].mPlayerText->SetTextColor(color);
		mArrRankWidgetGroupByMap[i].mPlayerText->SetEnable(true);
		mArrRankWidgetGroupByMap[i].mDistanceText->SetText((FormatWithCommaManual(_info.Distance) + L"m").c_str());
		mArrRankWidgetGroupByMap[i].mDistanceText->SetTextColor(color);
		mArrRankWidgetGroupByMap[i].mDistanceText->SetEnable(true);

		std::vector<int> _arrItemType = { _info.Item_0, _info.Item_1, _info.Item_2 };

		for (int j = 0; j < _arrItemType.size(); j++)
		{
			int _itemType = _arrItemType[j];
			bool _isEnable = _itemType != PLAYER_ITEM_TYPE_DEFAULT_INDEX;
			auto _image = mArrRankWidgetGroupByMap[i].mArrPlayerItemImage[j];
			auto _slotImage = mArrRankWidgetGroupByMap[i].mArrPlayerSlotImage[j];

			_slotImage->SetEnable(true);
			_image->SetEnable(_isEnable);

			if (_isEnable)
				_image->SetTexture(mArrItemImageName[_itemType], mArrItemImagePath[_itemType]);

			_image->SetColor(color);
			_slotImage->SetColor(color);
		}
	}
}

void CLobbyWidget::AddListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;

	sceneNetController->AddListener(this);
}

void CLobbyWidget::RemoveListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->RemoveListener(this);
}

void CLobbyWidget::ProcessMessage(const RecvMessage& msg)
{
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_DISCONNECT:
	case (int)ServerMessage::MSG_NEW_OWNER:
	case (int)ServerMessage::MSG_JOIN:
	case (int)ServerMessage::MSG_CHANGE_NICKNAME:
	case (int)ServerMessage::MSG_READY:
	case (int)ServerMessage::MSG_UNREADY:
	case (int)ServerMessage::MSG_PICK_CHARACTER:
	case (int)ServerMessage::MSG_PICK_MAP:
	case (int)ServerMessage::MSG_PICK_ITEM:
	{
		UpdateOtherPlayerInfo();

		bool _isHost = CMultiplayManager::GetInst()->GetIsHost();
		mMapLeftButton->SetEnable(_isHost);
		mMapRightButton->SetEnable(_isHost);

		curDifficultyIndex = CMultiplayManager::GetInst()->GetCurMapIndex();
		CDataStorageManager::GetInst()->SetSelectedMapIndex(curDifficultyIndex);
		UpdateMapInfo();

		break;
	}
	case (int)ServerMessage::MSG_START_ACK:
	{
		if (CMultiplayManager::GetInst()->GetIsGameStart())
			StartGame();

		break;
	}
	default:
		break;
	}
}

void CLobbyWidget::OnCharacterLeftButtonClick()
{
	CLog::PrintLog("CharacterLeftButtonClick()");

	if (curPlayerGraphicIndex > 0)
		curPlayerGraphicIndex--;
	else
		curPlayerGraphicIndex = CDataStorageManager::GetInst()->GetCharacterCount() - 1;

	CDataStorageManager::GetInst()->SetSelectedCharacterIndex(curPlayerGraphicIndex);
	UpdatePlayerStat();

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTriggerInt(ClientMessage::MSG_PICK_CHARACTER, curPlayerGraphicIndex);
	else
		UpdateSimpleCharacterRankInfo();
}

void CLobbyWidget::OnCharacterRightButtonClick()
{
	CLog::PrintLog("CharacterRightButtonClick()");

	curPlayerGraphicIndex++;

	if (curPlayerGraphicIndex == CDataStorageManager::GetInst()->GetCharacterCount())
		curPlayerGraphicIndex = 0;

	CDataStorageManager::GetInst()->SetSelectedCharacterIndex(curPlayerGraphicIndex);
	UpdatePlayerStat();

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTriggerInt(ClientMessage::MSG_PICK_CHARACTER, curPlayerGraphicIndex);
	else
		UpdateSimpleCharacterRankInfo();
}

void CLobbyWidget::InitOtherPlayersInfo()
{
	// title 100, 570
	auto titleTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("PlayerTitle");
	AddWidget(titleTextBlock);
	titleTextBlock->SetPivot(FVector2D(0.0f, 1.0f));
	titleTextBlock->SetSize(FVector2D(500.0f, 50.0f));
	titleTextBlock->SetPos(FVector2D(100.0f, 590.0f));
	titleTextBlock->SetText(PLAYER_TITLE_TEXT);
	titleTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
	titleTextBlock->SetAlignH(ETextAlignH::Left);
	titleTextBlock->SetFontSize(50.0f);
	titleTextBlock->SetShadowEnable(true);
	titleTextBlock->SetShadowOffset(3.f, 3.f);
	titleTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));

	// 120.0f, 490.0f
	float fontSize = 30.0f;
	FVector2D textBasePivot = FVector2D(0.0f, 0.4f);
	FVector2D textBaseSize = FVector2D(220.0f, fontSize + 10.0f);
	FVector2D textBasePos = FVector2D(70.0f, 510.0f);

	FVector2D hostBasePivot = FVector2D(1.0f, 0.5f);
	// textBaseSize 높이보단 조금 작게
	FVector2D hostBaseSize = FVector2D(fontSize, fontSize) + FVector2D::One * 0.5f;
	FVector2D hostBasePos = textBasePos + FVector2D(-5.0f, 0.0f);

	FVector2D itemBasePivot = FVector2D(0.5f, 0.5f);
	// textBaseSize 높이보단 조금 작게
	FVector2D itemBaseSize = hostBaseSize;
	FVector2D itemBasePos = textBasePos + FVector2D(textBaseSize.x * 1.5f + itemBaseSize.x * 0.5f, 0.0f);

	FVector2D playerCheckBackPivot = FVector2D(0.0f, 0.5f);
	FVector2D playerCheckBackSize = textBaseSize + FVector2D(210.0f, 0.0f);
	FVector2D playerCheckBackPos = textBasePos;

	mArrPlayerWidgetGroup.resize(PLAYER_COUNT_MAX);

	for (int i = 0; i < mArrPlayerWidgetGroup.size(); i++)
	{
		FPlayerWidgetGroup group;

		auto tempPlayerCheckBackImage = mScene->GetUIManager()->CreateWidget<CImage>("PlayerCheckBackImage_" + std::to_string(i));
		AddWidget(tempPlayerCheckBackImage);
		tempPlayerCheckBackImage->SetPivot(playerCheckBackPivot);
		tempPlayerCheckBackImage->SetSize(playerCheckBackSize);
		tempPlayerCheckBackImage->SetPos(playerCheckBackPos - FVector2D::Axis[EAxis::Y] * (textBaseSize.y) * i);
		tempPlayerCheckBackImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
		tempPlayerCheckBackImage->SetColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		tempPlayerCheckBackImage->SetZOrder(ZORDER_LOBBY_PLAYER_CHECK_BACK);
		tempPlayerCheckBackImage->SetEnable(false);
		group.mPlayerCheckBackImage = tempPlayerCheckBackImage;

		auto tempTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("PlayerText_" + std::to_string(i));
		AddWidget(tempTextBlock);
		tempTextBlock->SetPivot(textBasePivot);
		tempTextBlock->SetSize(textBaseSize);
		tempTextBlock->SetPos(textBasePos - FVector2D::Axis[EAxis::Y] * (textBaseSize.y) * i);
		tempTextBlock->SetText(PLAYER_EMPTY_TEXT);
		tempTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_White));
		tempTextBlock->SetOpacity(0.5f);
		tempTextBlock->SetAlignH(ETextAlignH::Left);
		tempTextBlock->SetFontSize(fontSize);
		tempTextBlock->SetShadowEnable(false);
		tempTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		tempTextBlock->SetZOrder(ZORDER_LOBBY_PLAYER);
		group.mPlayerText = tempTextBlock;

		auto tempHostImage = mScene->GetUIManager()->CreateWidget<CImage>("HostImage_" + std::to_string(i));
		AddWidget(tempHostImage);
		tempHostImage->SetPivot(hostBasePivot);
		tempHostImage->SetSize(hostBaseSize);
		tempHostImage->SetPos(hostBasePos - FVector2D::Axis[EAxis::Y] * (textBaseSize.y) * i);
		tempHostImage->SetTexture(TEXTURE_HOST_ICON_NAME, TEXTURE_HOST_ICON_PATH);
		tempHostImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
		tempHostImage->SetZOrder(ZORDER_LOBBY_PLAYER);
		tempHostImage->SetEnable(false);
		group.mPlayerHostImage = tempHostImage;

		for (int j = 0; j < itemSlotCount; j++)
		{
			auto tempItemSlotImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemSlotImage_" + std::to_string(i) + "_" + std::to_string(j));
			AddWidget(tempItemSlotImage);
			tempItemSlotImage->SetPivot(itemBasePivot);
			tempItemSlotImage->SetSize(itemBaseSize);
			tempItemSlotImage->SetPos(itemBasePos + FVector2D(textBaseSize.y * 0.8f * j, (textBaseSize.y) * i * -1.0f));
			tempItemSlotImage->SetTexture(ITEM_EMPTY_SQUARE_NAME, ITEM_EMPTY_SQUARE_PATH);
			tempItemSlotImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
			tempItemSlotImage->SetZOrder(ZORDER_LOBBY_PLAYER);
			group.mArrPlayerSlotImage.push_back(tempItemSlotImage);

			auto tempItemImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemaImage_" + std::to_string(i) + "_" + std::to_string(j));
			AddWidget(tempItemImage);
			tempItemImage->SetPivot(itemBasePivot);
			tempItemImage->SetSize(itemBaseSize * mSlotInnerItemSizeRate);
			tempItemImage->SetPos(itemBasePos + FVector2D(textBaseSize.y * 0.8f * j, (textBaseSize.y) * i * -1.0f));
			tempItemImage->SetTexture(ITEM_HP_ICON_NAME, ITEM_HP_ICON_PATH);
			tempItemImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
			tempItemImage->SetZOrder(ZORDER_LOBBY_PLAYER_ITEM_ICON);
			tempItemImage->SetEnable(false);
			group.mArrPlayerItemImage.push_back(tempItemImage);
		}
		mArrPlayerWidgetGroup[i] = group;
	}
}

void CLobbyWidget::InitSimpleCharacterRankInfo()
{
	auto titleTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("titleTextBlock");
	AddWidget(titleTextBlock);
	titleTextBlock->SetPivot(FVector2D(0.5f, 0.5f));
	titleTextBlock->SetSize(FVector2D(500.0f, 70.0f));
	titleTextBlock->SetPos(FVector2D(280.0f, 580.0f));
	titleTextBlock->SetText(PLAYER_CHARACTER_RANK_TEXT);
	titleTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
	titleTextBlock->SetAlignH(ETextAlignH::Center);
	titleTextBlock->SetFontSize(50.0f);
	titleTextBlock->SetShadowEnable(true);
	titleTextBlock->SetShadowOffset(3.f, 3.f);
	titleTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));

	float fontSize = 30.0f;
	float gapX = 10.0f;

	FVector2D nameTextBasePivot = FVector2D(0.0f, 0.4f);
	FVector2D nameTextBaseSize = FVector2D(220.0f, fontSize + 10.0f);
	FVector2D nameTextBasePos = FVector2D(40.0f, 510.0f);

	FVector2D distTextBasePivot = FVector2D(0.0f, 0.4f);
	FVector2D distTextBaseSize = FVector2D(150.0f, fontSize + 10.0f);
	FVector2D distTextBasePos = nameTextBasePos + FVector2D(nameTextBaseSize.x + gapX, 0.0f);

	FVector2D itemBasePivot = FVector2D(0.5f, 0.5f);
	// textBaseSize 높이보단 조금 작게
	FVector2D itemBaseSize = FVector2D(fontSize, fontSize) + FVector2D::One * 0.5f;
	FVector2D itemBasePos = distTextBasePos + FVector2D(distTextBaseSize.x + itemBaseSize.x * 0.5f, 0.0f);

	mArrRankWidgetGroupByCharacter.resize(PLAYER_SIMPLE_RANK);

	for (int i = 0; i < mArrRankWidgetGroupByCharacter.size(); i++)
	{
		FRankWidgetGroup group;

		auto nameTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("nameTextBlock_" + std::to_string(i));
		AddWidget(nameTextBlock);
		nameTextBlock->SetPivot(nameTextBasePivot);
		nameTextBlock->SetSize(nameTextBaseSize);
		nameTextBlock->SetPos(nameTextBasePos - FVector2D::Axis[EAxis::Y] * (nameTextBaseSize.y) * i);
		nameTextBlock->SetText(PLAYER_EMPTY_TEXT);
		nameTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_White));
		nameTextBlock->SetAlignH(ETextAlignH::Left);
		nameTextBlock->SetFontSize(fontSize);
		nameTextBlock->SetShadowEnable(false);
		nameTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		nameTextBlock->SetZOrder(ZORDER_LOBBY_PLAYER);
		group.mPlayerText = nameTextBlock;

		auto distTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("distTextBlock_" + std::to_string(i));
		AddWidget(distTextBlock);
		distTextBlock->SetPivot(distTextBasePivot);
		distTextBlock->SetSize(distTextBaseSize);
		distTextBlock->SetPos(distTextBasePos - FVector2D::Axis[EAxis::Y] * (distTextBaseSize.y) * i);
		distTextBlock->SetText((FormatWithCommaManual(12345) + L"m").c_str());
		distTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_White));
		distTextBlock->SetAlignH(ETextAlignH::Left);
		distTextBlock->SetFontSize(fontSize);
		distTextBlock->SetShadowEnable(false);
		distTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		distTextBlock->SetZOrder(ZORDER_LOBBY_PLAYER);
		group.mDistanceText = distTextBlock;

		for (int j = 0; j < itemSlotCount; j++)
		{
			auto tempItemSlotImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemSlotImage_" + std::to_string(i) + "_" + std::to_string(j));
			AddWidget(tempItemSlotImage);
			tempItemSlotImage->SetPivot(itemBasePivot);
			tempItemSlotImage->SetSize(itemBaseSize);
			tempItemSlotImage->SetPos(itemBasePos + FVector2D(nameTextBaseSize.y * 0.8f * j, (nameTextBaseSize.y) * i * -1.0f));
			tempItemSlotImage->SetTexture(ITEM_EMPTY_SQUARE_NAME, ITEM_EMPTY_SQUARE_PATH);
			tempItemSlotImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
			tempItemSlotImage->SetZOrder(ZORDER_LOBBY_PLAYER);
			group.mArrPlayerSlotImage.push_back(tempItemSlotImage);

			auto tempItemImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemaImage_" + std::to_string(i) + "_" + std::to_string(j));
			AddWidget(tempItemImage);
			tempItemImage->SetPivot(itemBasePivot);
			tempItemImage->SetSize(itemBaseSize * mSlotInnerItemSizeRate);
			tempItemImage->SetPos(itemBasePos + FVector2D(nameTextBaseSize.y * 0.8f * j, (nameTextBaseSize.y) * i * -1.0f));
			tempItemImage->SetTexture(ITEM_HP_ICON_NAME, ITEM_HP_ICON_PATH);
			tempItemImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
			tempItemImage->SetZOrder(ZORDER_LOBBY_PLAYER_ITEM_ICON);
			tempItemImage->SetEnable(false);
			group.mArrPlayerItemImage.push_back(tempItemImage);
		}

		mArrRankWidgetGroupByCharacter[i] = group;
	}
}

void CLobbyWidget::InitSimpleMapRankInfo()
{
	auto titleTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("titleTextBlock");
	AddWidget(titleTextBlock);
	titleTextBlock->SetPivot(FVector2D(0.5f, 0.5f));
	titleTextBlock->SetSize(FVector2D(500.0f, 70.0f));
	titleTextBlock->SetPos(FVector2D(280.0f, 360.0f));
	titleTextBlock->SetText(PLAYER_MAP_RANK_TEXT);
	titleTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
	titleTextBlock->SetAlignH(ETextAlignH::Center);
	titleTextBlock->SetFontSize(50.0f);
	titleTextBlock->SetShadowEnable(true);
	titleTextBlock->SetShadowOffset(3.f, 3.f);
	titleTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));

	float fontSize = 30.0f;
	float gapX = 10.0f;

	FVector2D nameTextBasePivot = FVector2D(0.0f, 0.4f);
	FVector2D nameTextBaseSize = FVector2D(220.0f, fontSize + 10.0f);
	FVector2D nameTextBasePos = FVector2D(40.0f, 290.0f);

	FVector2D distTextBasePivot = FVector2D(0.0f, 0.4f);
	FVector2D distTextBaseSize = FVector2D(150.0f, fontSize + 10.0f);
	FVector2D distTextBasePos = nameTextBasePos + FVector2D(nameTextBaseSize.x + gapX, 0.0f);

	FVector2D itemBasePivot = FVector2D(0.5f, 0.5f);
	// textBaseSize 높이보단 조금 작게
	FVector2D itemBaseSize = FVector2D(fontSize, fontSize) + FVector2D::One * 0.5f;
	FVector2D itemBasePos = distTextBasePos + FVector2D(distTextBaseSize.x + itemBaseSize.x * 0.5f, 0.0f);

	mArrRankWidgetGroupByMap.resize(PLAYER_SIMPLE_RANK);

	for (int i = 0; i < mArrRankWidgetGroupByMap.size(); i++)
	{
		FRankWidgetGroup group;

		auto nameTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("nameTextBlock_" + std::to_string(i));
		AddWidget(nameTextBlock);
		nameTextBlock->SetPivot(nameTextBasePivot);
		nameTextBlock->SetSize(nameTextBaseSize);
		nameTextBlock->SetPos(nameTextBasePos - FVector2D::Axis[EAxis::Y] * (nameTextBaseSize.y) * i);
		nameTextBlock->SetText(PLAYER_EMPTY_TEXT);
		nameTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_White));
		nameTextBlock->SetAlignH(ETextAlignH::Left);
		nameTextBlock->SetFontSize(fontSize);
		nameTextBlock->SetShadowEnable(false);
		nameTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		nameTextBlock->SetZOrder(ZORDER_LOBBY_PLAYER);
		group.mPlayerText = nameTextBlock;

		auto distTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("distTextBlock_" + std::to_string(i));
		AddWidget(distTextBlock);
		distTextBlock->SetPivot(distTextBasePivot);
		distTextBlock->SetSize(distTextBaseSize);
		distTextBlock->SetPos(distTextBasePos - FVector2D::Axis[EAxis::Y] * (distTextBaseSize.y) * i);
		distTextBlock->SetText((FormatWithCommaManual(12345) + L"m").c_str());
		distTextBlock->SetTextColor(FVector4D::GetColorFromString(COLOR_White));
		distTextBlock->SetAlignH(ETextAlignH::Left);
		distTextBlock->SetFontSize(fontSize);
		distTextBlock->SetShadowEnable(false);
		distTextBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		distTextBlock->SetZOrder(ZORDER_LOBBY_PLAYER);
		group.mDistanceText = distTextBlock;

		for (int j = 0; j < itemSlotCount; j++)
		{
			auto tempItemSlotImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemSlotImage_" + std::to_string(i) + "_" + std::to_string(j));
			AddWidget(tempItemSlotImage);
			tempItemSlotImage->SetPivot(itemBasePivot);
			tempItemSlotImage->SetSize(itemBaseSize);
			tempItemSlotImage->SetPos(itemBasePos + FVector2D(nameTextBaseSize.y * 0.8f * j, (nameTextBaseSize.y) * i * -1.0f));
			tempItemSlotImage->SetTexture(ITEM_EMPTY_SQUARE_NAME, ITEM_EMPTY_SQUARE_PATH);
			tempItemSlotImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
			tempItemSlotImage->SetZOrder(ZORDER_LOBBY_PLAYER);
			group.mArrPlayerSlotImage.push_back(tempItemSlotImage);

			auto tempItemImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemaImage_" + std::to_string(i) + "_" + std::to_string(j));
			AddWidget(tempItemImage);
			tempItemImage->SetPivot(itemBasePivot);
			tempItemImage->SetSize(itemBaseSize * mSlotInnerItemSizeRate);
			tempItemImage->SetPos(itemBasePos + FVector2D(nameTextBaseSize.y * 0.8f * j, (nameTextBaseSize.y) * i * -1.0f));
			tempItemImage->SetTexture(ITEM_HP_ICON_NAME, ITEM_HP_ICON_PATH);
			tempItemImage->SetColor(FVector4D::GetColorFromString(COLOR_White));
			tempItemImage->SetZOrder(ZORDER_LOBBY_PLAYER_ITEM_ICON);
			tempItemImage->SetEnable(false);
			group.mArrPlayerItemImage.push_back(tempItemImage);
		}

		mArrRankWidgetGroupByMap[i] = group;
	}
}

void CLobbyWidget::UpdatePlayerStat()
{
	auto playerController = dynamic_cast<IScenePlayerGraphicController*>(mScene);

	if (playerController)
	{
		playerController->SetChangeGraphic(0, curPlayerGraphicIndex);
	}

	int count = (int)ECharacterStatText::End;
	FCharacterState stat = CDataStorageManager::GetInst()->GetCharacterState(curPlayerGraphicIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(ECharacterStatText::gArrCharacterStatText[i]) + L" :";
		auto valueText = stat.GetStatToWString(static_cast<ECharacterStatText::Type>(i));

		mArrPlayerStatNameText[i]->SetText(nameText.c_str());

		if (mArrItemStatAddValue[i] > 0.0f)
		{
			mArrPlayerStatValueText[i]->SetTextColor(FVector4D::GetColorFromString(COLOR_Red));
			wchar_t _wc_str[64] = {};
			swprintf_s(_wc_str, L"%s(+%.0f)", valueText.c_str(), mArrItemStatAddValue[i]);
			mArrPlayerStatValueText[i]->SetText(_wc_str);
		}
		else
		{
			mArrPlayerStatValueText[i]->SetTextColor(FVector4D::GetColorFromString(stat.ColorName));
			mArrPlayerStatValueText[i]->SetText(valueText.c_str());
		}
	}
}

void CLobbyWidget::OnMapLeftButtonClick()
{
	CLog::PrintLog("MapLeftButtonClick()");

	curDifficultyIndex--;

	if (curDifficultyIndex < 0)
		curDifficultyIndex = CDataStorageManager::GetInst()->GetMapInfoCount() - 1;

	CDataStorageManager::GetInst()->SetSelectedMapIndex(curDifficultyIndex);
	UpdateMapInfo();

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTriggerInt(ClientMessage::MSG_PICK_MAP, curDifficultyIndex);
	else
		UpdateSimpleMapRankInfo();
}

void CLobbyWidget::OnMapRightButtonClick()
{
	CLog::PrintLog("MapRightButtonClick()");

	curDifficultyIndex++;

	if (curDifficultyIndex == CDataStorageManager::GetInst()->GetMapInfoCount())
		curDifficultyIndex = 0;

	CDataStorageManager::GetInst()->SetSelectedMapIndex(curDifficultyIndex);
	UpdateMapInfo();

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTriggerInt(ClientMessage::MSG_PICK_MAP, curDifficultyIndex);
	else
		UpdateSimpleMapRankInfo();
}

void CLobbyWidget::UpdateMapInfo()
{
	mMapDifficultyImage->SetTexture(mArrMapDifficultyImageName[curDifficultyIndex]
		, mArrMapDifficultyImagePath[curDifficultyIndex]);
	mMapDifficultyImage->SetColor(mArrMapDifficultyImageColor[curDifficultyIndex]);

	int count = (int)EMapInfoText::End;
	FMapInfo info = CDataStorageManager::GetInst()->GetMapInfo(curDifficultyIndex);

	for (int i = 0; i < count; i++)
	{
		auto nameText = std::wstring(EMapInfoText::gArrMapInfoText[i]) + L" :";
		auto valueText = info.GetInfoToWString(static_cast<EMapInfoText::Type>(i));

		mArrMapInfoNameText[i]->SetText(nameText.c_str());
		mArrMapInfoValueText[i]->SetTextColor(FVector4D::GetColorFromString(info.ColorName));
		mArrMapInfoValueText[i]->SetText(valueText.c_str());
	}
}