#include "MouseWidget.h"
#include "../Common/Image.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Input.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneManager.h"

CMouseWidget::CMouseWidget()
{
}

CMouseWidget::~CMouseWidget()
{
}

bool CMouseWidget::Init()
{
    CUserWidget::Init();

    mImage = CSceneUIManager::CreateWidgetStatic<CImage>("Mouse");

    mImage->SetTexture("MouseDefault", 
        TEXT("Texture\\Mouse\\Default\\0.png"));

    mImage->SetSize(32.f, 32.f);

    AddWidget(mImage);

    return true;
}

void CMouseWidget::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);

	CInput* Input = 
		CSceneManager::GetInst()->GetCurrentScene()->GetInput();

	FVector2D MousePos = Input->GetMousePos();

	SetPos(MousePos.x, MousePos.y - 32.f);
}
