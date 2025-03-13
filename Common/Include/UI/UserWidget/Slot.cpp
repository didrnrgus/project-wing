#include "Slot.h"
#include "../Common/Image.h"
#include "../Common/TextBlock.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneUIManager.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/UICBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/Shader.h"

CSlot::CSlot()
{
}

CSlot::~CSlot()
{
}

void CSlot::SetTextureBack(const std::string& Name)
{
    mBack->SetTexture(Name);
}

void CSlot::SetTextureBack(const std::string& Name,
    const TCHAR* FileName)
{
    mBack->SetTexture(Name, FileName);
}

void CSlot::SetTextureBack(CTexture* Texture)
{
    mBack->SetTexture(Texture);
}

void CSlot::SetTintBack(float r, float g, float b)
{
    mBack->SetTint(r, g, b);
}

void CSlot::SetOpacityBack(float Opacity)
{
    mBack->SetOpacity(Opacity);
}

void CSlot::SetTextureIcon(const std::string& Name)
{
    mIcon->SetTexture(Name);

    mIcon->SetOpacity(1.f);
}

void CSlot::SetTextureIcon(const std::string& Name,
    const TCHAR* FileName)
{
    mIcon->SetTexture(Name, FileName);

    mIcon->SetOpacity(1.f);
}

void CSlot::SetTextureIcon(CTexture* Texture)
{
    mIcon->SetTexture(Texture);

    if (Texture)
        mIcon->SetOpacity(1.f);

    else
        mIcon->SetOpacity(0.f);
}

void CSlot::SetTintIcon(float r, float g, float b)
{
    mIcon->SetTint(r, g, b);
}

void CSlot::SetOpacityIcon(float Opacity)
{
    mIcon->SetOpacity(Opacity);
}

void CSlot::SetSize(const FVector2D& Size)
{
    CUserWidget::SetSize(Size);

    mBack->SetSize(Size);
    mIcon->SetSize(Size - 8.f);
}

void CSlot::SetSize(float x, float y)
{
    CUserWidget::SetSize(x, y);

    mBack->SetSize(x, y);
    mIcon->SetSize(x - 8.f, y - 8.f);
}

void CSlot::SetPos(const FVector2D& Pos)
{
    CUserWidget::SetPos(Pos);

    mIcon->SetPos(4.f, 4.f);
}

void CSlot::SetPos(float x, float y)
{
    CUserWidget::SetPos(x, y);

    mIcon->SetPos(4.f, 4.f);
}

bool CSlot::Init()
{
    CUserWidget::Init();

    mBack = mScene->GetUIManager()->CreateWidget<CImage>("Back");

    mBack->SetTexture("SlotBack", TEXT("Texture/SlotBack.png"));

    AddWidget(mBack);

    mIcon = mScene->GetUIManager()->CreateWidget<CImage>("Icon");

    mIcon->SetOpacity(0.f);

    AddWidget(mIcon);

    return true;
}

void CSlot::Update(float DeltaTime)
{
    CUserWidget::Update(DeltaTime);
}

void CSlot::Render()
{
    CUserWidget::Render();
}
