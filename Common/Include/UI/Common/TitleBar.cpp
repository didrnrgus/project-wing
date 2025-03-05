#include "TitleBar.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "../../Scene/Input.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/UICBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/Shader.h"
#include "TextBlock.h"
#include "../../Scene/SceneUIManager.h"

CTitleBar::CTitleBar()
{
}

CTitleBar::~CTitleBar()
{
}

void CTitleBar::SetTexture(const std::string& Name)
{
    if (mScene)
        mBrush.Texture = mScene->GetAssetManager()->FindTexture(Name);

    else
        mBrush.Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
}

void CTitleBar::SetTexture(const std::string& Name,
    const TCHAR* FileName)
{
    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
            return;

        mBrush.Texture = mScene->GetAssetManager()->FindTexture(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
            return;

        mBrush.Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
    }
}

void CTitleBar::SetTexture(CTexture* Texture)
{
    if (mScene)
        mBrush.Texture = Texture;

    else
        mBrush.Texture = Texture;
}

void CTitleBar::SetTint(float r, float g, float b)
{
    mBrush.Tint.x = r;
    mBrush.Tint.y = g;
    mBrush.Tint.z = b;
}

void CTitleBar::SetOpacity(float Opacity)
{
    mBrush.Tint.w = Opacity;
}

void CTitleBar::SetText(const TCHAR* Text)
{
    mTextBlock->SetText(Text);
}

void CTitleBar::SetSize(const FVector2D& Size)
{
    CWidget::SetSize(Size);

    mTextBlock->SetSize(Size);
}

void CTitleBar::SetSize(float x, float y)
{
    CWidget::SetSize(x, y);

    mTextBlock->SetSize(x, y);
}

bool CTitleBar::Init()
{
    CWidget::Init();

    mTextBlock = mScene->GetUIManager()->CreateWidget<CTextBlock>("Name");

    mTextBlock->SetText(TEXT("Title"));
    mTextBlock->SetTextColor(0, 0, 0, 255);
    mTextBlock->SetAlignH(ETextAlignH::Center);
    mTextBlock->SetParent(this);

    return true;
}

void CTitleBar::Update(float DeltaTime)
{
    CWidget::Update(DeltaTime);

    if (mMouseHovered && mUpdateWidget)
    {
        if (mScene->GetInput()->GetMouseDown(EMouseButtonType::LButton))
        {
            mMouseDrag = true;
        }

        else if (mScene->GetInput()->GetMouseHold(EMouseButtonType::LButton))
        {
            if (mMouseDrag)
            {
                const FVector2D& MouseMove = mScene->GetInput()->GetMouseMove();

                mUpdateWidget->AddPos(MouseMove);
            }
        }

        else if (mScene->GetInput()->GetMouseUp(EMouseButtonType::LButton))
        {
            mMouseDrag = false;
            //mMouseHovered = false;
        }
    }
}

void CTitleBar::Render()
{
    CWidget::Render();

    FMatrix matScale, matRot, matTranslate, matWorld;

    matScale.Scaling(mSize);
    matRot.RotationZ(mRotation);
    matTranslate.Translation(mRenderPos);

    matWorld = matScale * matRot * matTranslate;

    mTransformCBuffer->SetWorldMatrix(matWorld);
    mTransformCBuffer->SetProjMatrix(mUIProj);
    mTransformCBuffer->SetPivot(mPivot);

    mTransformCBuffer->UpdateBuffer();

    //mUICBuffer->SetWidgetColor(mColor);

    mUICBuffer->SetTint(mBrush.Tint);

    if (mBrush.Texture)
    {
        mUICBuffer->SetTextureEnable(true);

        mBrush.Texture->SetShader(0, EShaderBufferType::Pixel,
            0);
    }

    else
        mUICBuffer->SetTextureEnable(false);

    if (mBrush.AnimationEnable)
    {
        mUICBuffer->SetAnimationEnable(true);

        int Frame = mBrush.Frame;

        FAnimationFrame FrameInfo = mBrush.Frames[Frame];
        mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
            FrameInfo.Start.x + FrameInfo.Size.x,
            FrameInfo.Start.y + FrameInfo.Size.y);
    }

    else
        mUICBuffer->SetAnimationEnable(false);


    mUICBuffer->UpdateBuffer();

    mShader->SetShader();

    mMesh->Render();

    mTextBlock->Render();
}

void CTitleBar::Render(const FVector3D& Pos)
{
    CWidget::Render(Pos);
}

void CTitleBar::MouseHovered()
{
    mMouseHovered = true;
}

void CTitleBar::MouseUnHovered()
{
    if (!mMouseDrag)
        mMouseHovered = false;
}
