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

	hpBackImage->SetTexture("Texture/basic.png");
	hpBackImage->SetPivot(pivot);
	hpBackImage->SetSize(backSize);
	hpBackImage->SetColor(FVector4D::Gray70);
	hpBackImage->SetOpacity(0.5f);
	hpBackImage->SetPos(backPos);

	mHpFrontImage->SetTexture("Texture/basic.png");
	mHpFrontImage->SetPivot(pivot);
	mHpFrontImage->SetSize(mHpFrontSize);
	mHpFrontImage->SetColor(FVector4D::Red);
	mHpFrontImage->SetOpacity(0.5f);
	mHpFrontImage->SetPos(frontPos);

	auto inGameScene = dynamic_cast<CSceneInGame*>(mScene);
	int myPlayerIndex = CDataStorageManager::GetInst()->GetSelectedCharacterIndex();

	if (inGameScene)
	{
		auto player = inGameScene->GetPlayer(myPlayerIndex);
		mPlayerStat = dynamic_cast<IPlayerStatController*>(player);

		if (mPlayerStat == nullptr)
		{
			CLog::PrintLog("CInGameWidget::Init (mPlayerStat == nullptr) Error");
		}
	}

	return true;
}

void CInGameWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (mPlayerStat)
		UpdateTargetPlayerStat(DeltaTime);
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
