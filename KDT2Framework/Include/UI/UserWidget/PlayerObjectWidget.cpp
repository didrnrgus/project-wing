#pragma once
#include "PlayerObjectWidget.h"
#include "UI/Common/TextBlock.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Etc/ZOrderContainer.h"
#include "Etc/MultiplayManager.h"
#include "Etc/NetworkManager.h"
#include "Etc/DataStorageManager.h"

CPlayerObjectWidget::CPlayerObjectWidget()
{
}

CPlayerObjectWidget::~CPlayerObjectWidget()
{
}

bool CPlayerObjectWidget::Init()
{
	CUserWidget::Init();
	mInfoText = mScene->GetUIManager()->CreateWidget<CTextBlock>("InfoText");
	AddWidget(mInfoText);
	mInfoText->SetPivot(FVector2D(0.0f, 1.0f));
	mInfoText->SetSize(FVector2D(100.0f, 200.0f));
	mInfoText->SetPos(FVector2D(0.0f, 0.0f));
	mInfoText->SetText(L"InfoText");
	mInfoText->SetTextColor(FVector4D::White);
	mInfoText->SetAlignH(ETextAlignH::Left);
	mInfoText->SetFontSize(30.0f);
	mInfoText->SetShadowEnable(true);
	mInfoText->SetShadowOffset(3.f, 3.f);
	mInfoText->SetTextShadowColor(FVector4D::Gray30);
	mInfoText->SetZOrder(ZORDER_INGAME_PLAY_OTHER_NAME_TAG);
	return true;
}

void CPlayerObjectWidget::Update(float DeltaTime)
{
	CUserWidget::Update(DeltaTime);

	if (CNetworkManager::GetInst()->IsMultiplay() && mObjectNetworkController)
	{
		if (!CMultiplayManager::GetInst()->GetIsGameStart())
			return;

		auto playerInfo = CMultiplayManager::GetInst()->GetPlayerInfoValueById(mObjectNetworkController->GetNetID());
		wchar_t	_InfoText[64] = {};
		//swprintf_s(_InfoText, L"[P%d]\nHP: %.0f\nDist: %.0fm", playerInfo.id, playerInfo.curHp, playerInfo.distance);
		//swprintf_s(_InfoText, L"[P%d] HP: %.0f | Dist: %.0fm", playerInfo.id, playerInfo.curHp, playerInfo.distance);
		swprintf_s(_InfoText, L"[P%d]", playerInfo.id);
		
		mInfoText->SetText(_InfoText);
		
		auto playerStat = CDataStorageManager::GetInst()->GetCharacterState(playerInfo.characterType);
		mInfoText->SetTextColor(FVector4D::GetColorFromString(playerStat.ColorName));
	}
}

