#include "HeadInfo.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"

CHeadInfo::CHeadInfo()
{
}

CHeadInfo::~CHeadInfo()
{
}

bool CHeadInfo::Init()
{
    mHPBar = mScene->GetUIManager()->CreateWidget<CProgressBar>("HPBar");
    mNameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("NameText");

    mHPBar->SetSize(100.f, 20.f);
    mHPBar->SetTint(EProgressBarImageType::Back, 0.3f, 0.3f, 0.3f);
    mHPBar->SetTexture(EProgressBarImageType::Fill, "HPBar",
        TEXT("Texture\\HPBar.png"));

    //mHPBar->SetPercent(0.7f);

    AddWidget(mHPBar);

    mNameText->SetText(TEXT("이름"));
    mNameText->SetTextColor(255, 255, 255, 255);
    mNameText->SetAlignH(ETextAlignH::Center);
    mNameText->SetFontSize(20.f);
    mNameText->SetShadowEnable(true);
    mNameText->SetShadowOffset(3.f, 3.f);
    mNameText->SetPos(0.f, 20.f);
    mNameText->SetSize(100.f, 40.f);

    AddWidget(mNameText);

    return true;
}

void CHeadInfo::Render()
{
    CUserWidget::Render();
}

void CHeadInfo::Render(const FVector3D& Pos)
{
    CUserWidget::Render(Pos);
}
