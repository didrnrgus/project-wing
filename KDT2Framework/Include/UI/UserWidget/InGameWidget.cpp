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

CInGameWidget::CInGameWidget()
{
}

CInGameWidget::~CInGameWidget()
{
}

bool CInGameWidget::Init()
{
	CUserWidget::Init();

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
	mHpFrontImage->SetColor(FVector4D::Red);
	mHpFrontImage->SetOpacity(0.5f);
	mHpFrontImage->SetPos(frontPos);

	auto inGameScene = dynamic_cast<CSceneInGame*>(mScene);
	if (inGameScene)
	{
		// 내가 조작하는 플레이어를 가져와서 거기에 붙은 스탯을 불러오는것.
		auto player = inGameScene->GetPlayer(0);
		mPlayerStat = dynamic_cast<IPlayerStatController*>(player);

		if (mPlayerStat == nullptr)
		{
			CLog::PrintLog("CInGameWidget::Init (mPlayerStat == nullptr) Error");
		}
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

	return true;
}

void CInGameWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (mPlayerStat)
		UpdateTargetPlayerStat(DeltaTime);
}

void CInGameWidget::InitSelectedItemSlot()
{
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
		buttonBackImage->SetColor(FVector4D::Green);
		buttonBackImage->SetPos(tempPos);

		// 슬롯 내부 아이템 이미지.
		int selectedItemIndex = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(i);
		CSharedPtr<CImage> buttonImage = mScene->GetUIManager()->CreateWidget<CImage>("ItmeImage_" + std::to_string(i));
		AddWidget(buttonImage);
		mItemImages.push_back(buttonImage);
		if (selectedItemIndex > -1)
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
		buttonImage->SetColor(FVector4D::Green);
		buttonImage->SetPos(tempPos);

	}
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
}
