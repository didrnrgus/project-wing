#include "InGameWidget.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneInGame.h"
#include "Scene/Input.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Scene/SceneTitle.h"
#include "Etc/DataStorageManager.h"
#include "Interface/IPlayerStatController.h"
#include "Object/PlayerInGameObject.h"

CInGameWidget::CInGameWidget()
{
}

CInGameWidget::~CInGameWidget()
{
}

bool CInGameWidget::Init()
{
	CSceneWidget::Init();

	auto mapInfo = CDataStorageManager::GetInst()->GetSelectedMapInfo();

	FResolution RS = CDevice::GetInst()->GetResolution();

	CSharedPtr<CImage> hpBackImage = mScene->GetUIManager()->CreateWidget<CImage>("HPBackImage");
	mHpFrontImage = mScene->GetUIManager()->CreateWidget<CImage>("HPImage");
	AddWidget(hpBackImage);
	AddWidget(mHpFrontImage);

	FVector2D pivot = FVector2D(0.0f, 1.0f);
	FVector2D backSize = FVector2D(RS.Width * 0.4f, 30.0f);
	mHpFrontSize = backSize + FVector2D(-10.0f, -10.0f);
	FVector2D backPos = FVector2D(40.0f, RS.Height - 40.0f);
	FVector2D frontPos = backPos + FVector2D(5.0f, -5.0f);

	hpBackImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	hpBackImage->SetPivot(pivot);
	hpBackImage->SetSize(backSize);
	hpBackImage->SetColor(FVector4D::Gray70);
	hpBackImage->SetOpacity(0.5f);
	hpBackImage->SetPos(backPos);

	mHpFrontImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mHpFrontImage->SetPivot(pivot);
	mHpFrontImage->SetSize(mHpFrontSize);
	mHpFrontImage->SetColor(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
	mHpFrontImage->SetOpacity(0.5f);
	mHpFrontImage->SetPos(frontPos);

	auto inGameScene = dynamic_cast<CSceneInGame*>(mScene);
	if (inGameScene)
	{
		// 내가 조작하는 플레이어를 가져와서 거기에 붙은 스탯을 불러오는것.
		auto player = inGameScene->GetMyPlayerObject();
		mPlayerStat = dynamic_cast<IPlayerStatController*>(player);

		if (mPlayerStat == nullptr)
		{
			CLog::PrintLog("CInGameWidget::Init (mPlayerStat == nullptr) Error");
		}
	}

	{ // 아이템 정보 세팅.
		// 아이템 아이콘 path setting
		auto itemDatas = CDataStorageManager::GetInst()->GetItemInfoDatas();
		int index = 0;
		for (auto& item : itemDatas)
		{
			mItemImagePaths.push_back(item.second.GetItmeImagePath(index));
			mItemImageNames.push_back(item.second.GetItmeImageName(index));
			index++;
		}

		itemSlotCount = CDataStorageManager::GetInst()->GetSelectableItemCount();

		// slot image path
		mSlotImagePaths.push_back(ITEM_ADD_SQUARE_PATH);
		mSlotImagePaths.push_back(ITEM_EMPTY_SQUARE_PATH);
		mSlotImageNames.push_back(ITEM_ADD_SQUARE_NAME);
		mSlotImageNames.push_back(ITEM_EMPTY_SQUARE_NAME);

		mSlotPosBase = FVector2D(100.0f, 100.0f);
		mSlotPosAdd = FVector2D(105.0f, 0.0f);
		mSlotSize = FVector2D(100.0f, 100.0f);
		mSlotInnerItemSizeRate = 0.8f;

		InitSelectedItemSlot();
	}

	{ // start count setting
		mStartCountTextArr.push_back(L"START!!");
		mStartCountTextArr.push_back(L"1");
		mStartCountTextArr.push_back(L"2");
		mStartCountTextArr.push_back(L"3");
		mStartCountTextArr.push_back(L"READY~");

		FVector2D pivot = FVector2D(0.5f, 0.5f);
		FVector2D size = FVector2D(1000.0f, 200.5f);
		FVector2D pos = FVector2D(RS.Width, RS.Height) * 0.5f;

		mStartCountText = mScene->GetUIManager()->CreateWidget<CTextBlock>("mStartCountText");
		AddWidget(mStartCountText);
		mStartCountText->SetPivot(pivot);
		mStartCountText->SetSize(size);
		mStartCountText->SetPos(pos);
		mStartCountText->SetText(mStartCountTextArr[mStartCountTextArr.size() - 1]);
		mStartCountText->SetTextColor(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
		mStartCountText->SetFontSize(200.0f);
		mStartCountText->SetAlignH(ETextAlignH::Center);
		mStartCountText->SetShadowEnable(true);
		mStartCountText->SetShadowOffset(10.0f, 10.0f);
		mStartCountText->SetTextShadowColor(FVector4D::Gray30);
		mStartCountText->SetZOrder(ZORDER_INGAME_START_COUNT_TEXT);
	}

	{ // play distance text setting
		// mPlayDistanceText
		FVector2D pivot = FVector2D(1.0f, 0.0f);
		FVector2D size = FVector2D(500.0f, 100.0f);
		FVector2D pos = FVector2D(RS.Width - 100.0f, RS.Height * 0.5f);

		mPlayDistanceText = mScene->GetUIManager()->CreateWidget<CTextBlock>("mPlayDistanceText");
		AddWidget(mPlayDistanceText);
		mPlayDistanceText->SetPivot(pivot);
		mPlayDistanceText->SetSize(size);
		mPlayDistanceText->SetPos(pos);
		mPlayDistanceText->SetText((std::to_wstring(0) + L"m").c_str());
		mPlayDistanceText->SetTextColor(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
		mPlayDistanceText->SetFontSize(100.0f);
		mPlayDistanceText->SetAlignH(ETextAlignH::Right);
		mPlayDistanceText->SetShadowEnable(true);
		mPlayDistanceText->SetShadowOffset(5.f, 5.f);
		mPlayDistanceText->SetTextShadowColor(FVector4D::Gray30);
		mPlayDistanceText->SetZOrder(ZORDER_INGAME_START_COUNT_TEXT);
		mPlayDistanceText->SetEnable(false);
	}

	return true;
}

void CInGameWidget::Update(float DeltaTime)
{
	CSceneWidget::Update(DeltaTime);

	if (mPlayerStat)
		UpdateTargetPlayerStat(DeltaTime);
}

void CInGameWidget::InitSelectedItemSlot()
{
	auto mapInfo = CDataStorageManager::GetInst()->GetSelectedMapInfo();

	for (int i = 0; i < itemSlotCount; i++)
	{
		// 슬롯 이미지.
		FVector2D tempPos = mSlotPosBase + mSlotPosAdd * i;
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemSlotBack");
		AddWidget(buttonBackImage);
		mItemSlotImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mSlotImageNames[(int)SlotType::Added]
			, mSlotImagePaths[(int)SlotType::Added]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize);
		buttonBackImage->SetColor(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
		buttonBackImage->SetPos(tempPos);

		// 슬롯 내부 아이템 이미지.
		int selectedItemIndex = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(i);
		CSharedPtr<CImage> buttonImage = mScene->GetUIManager()->CreateWidget<CImage>("ItmeImage_" + std::to_string(i));
		AddWidget(buttonImage);
		mItemImages.push_back(buttonImage);
		if (selectedItemIndex >= 0)
		{
			buttonImage->SetTexture(mItemImageNames[selectedItemIndex], mItemImagePaths[selectedItemIndex]);
			buttonImage->SetEnable(true);
		}
		else
		{
			buttonImage->SetEnable(false);
			continue;
		}
		buttonImage->SetPivot(FVector2D::One * 0.5f);
		buttonImage->SetSize(mSlotSize * mSlotInnerItemSizeRate * mSlotInnerItemSizeRate);
		buttonImage->SetColor(FVector4D::GetColorFromString(mapInfo.DifficultyColorName));
		buttonImage->SetPos(tempPos);

	}
}

void CInGameWidget::SetStartCount(int count)
{
	if (count < 0)
	{
		mStartCountText->SetEnable(false);
		mPlayDistanceText->SetEnable(true);
		return;
	}

	mStartCountText->SetText(mStartCountTextArr[count]);
}

void CInGameWidget::UpdateTargetPlayerStat(float DeltaTime)
{
	// HP Gauge
	float maxHp = mPlayerStat->GetMaxHP();
	float curHp = mPlayerStat->GetCurHP();
	float maxHpSizeX = mHpFrontSize.x;
	float curHpSizeX = maxHpSizeX * (curHp / maxHp);

	//CLog::PrintLog("CInGameWidget::UpdateTargetPlayerStat maxHp: " + std::to_string(maxHp));
	//CLog::PrintLog("CInGameWidget::UpdateTargetPlayerStat curHp: " + std::to_string(curHp));
	mHpFrontImage->SetSize(FVector2D(curHpSizeX, mHpFrontSize.y));

	// play distance.
	auto playDistanceStr = std::to_wstring((int)mPlayerStat->GetPlayDistance()) + L"m";
	mPlayDistanceText->SetText(playDistanceStr.c_str());
}
