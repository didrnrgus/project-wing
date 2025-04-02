#pragma once
#include "DebugWidget.h"
#include "UI/Common/TextBlock.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Etc/ZOrderContainer.h"

CDebugWidget::CDebugWidget()
{
}

CDebugWidget::~CDebugWidget()
{
}

bool CDebugWidget::Init()
{
	CUserWidget::Init();

	mDebugText = mScene->GetUIManager()->CreateWidget<CTextBlock>("DebugText");
	AddWidget(mDebugText);
	mDebugText->SetPivot(FVector2D(0.0f, 1.0f));
	mDebugText->SetSize(FVector2D(500.0, 200.0f));
	mDebugText->SetPos(FVector2D(0.0f, 0.0f));
	mDebugText->SetText(L"DebugText");
	mDebugText->SetTextColor(FVector4D::Red);
	mDebugText->SetAlignH(ETextAlignH::Left);
	mDebugText->SetFontSize(30.0f);
	mDebugText->SetShadowEnable(false);
	mDebugText->SetZOrder(ZORDER_FPS);
	return true;
}

void CDebugWidget::SetDebugText(const wchar_t* wstr)
{
	mDebugText->SetText(wstr);
}

const wchar_t* CDebugWidget::GetDebugText()
{
	return mDebugText->GetText();
}

