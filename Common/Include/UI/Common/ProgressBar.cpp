#include "ProgressBar.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/UICBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/Shader.h"
#include "../../Scene/CameraManager.h"

CProgressBar::CProgressBar()
{
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::SetTexture(EProgressBarImageType Type,
    const std::string& Name)
{
    CTexture* Texture = nullptr;

    if (mScene)
        Texture = mScene->GetAssetManager()->FindTexture(Name);

    else
        Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);

    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Texture = Texture;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Texture = Texture;
        break;
    }
}

void CProgressBar::SetTexture(EProgressBarImageType Type, const std::string& Name,
    const TCHAR* FileName)
{
    CTexture* Texture = nullptr;

    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
            return;

        Texture = mScene->GetAssetManager()->FindTexture(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
            return;

        Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
    }

    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Texture = Texture;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Texture = Texture;
        break;
    }
}

void CProgressBar::SetTexture(EProgressBarImageType Type, CTexture* Texture)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Texture = Texture;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Texture = Texture;
        break;
    }
}

void CProgressBar::SetTint(EProgressBarImageType Type, float r, float g, float b)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Tint.x = r;
        mBackBrush.Tint.y = g;
        mBackBrush.Tint.z = b;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Tint.x = r;
        mFillBrush.Tint.y = g;
        mFillBrush.Tint.z = b;
        break;
    }
}

void CProgressBar::SetOpacity(EProgressBarImageType Type, float Opacity)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Tint.w = Opacity;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Tint.w = Opacity;
        break;
    }
}

void CProgressBar::SetBrushAnimation(EProgressBarImageType Type, bool Animation)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.AnimationEnable = Animation;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.AnimationEnable = Animation;
        break;
    }
}

void CProgressBar::AddBrushFrame(EProgressBarImageType Type, const FVector2D& Start,
    const FVector2D& Size)
{
    FAnimationFrame Frame;
    Frame.Start = Start;
    Frame.Size = Size;

    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Frames.emplace_back(Frame);

        mBackBrush.FrameTime = mBackBrush.PlayTime /
            mBackBrush.Frames.size();
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Frames.emplace_back(Frame);

        mFillBrush.FrameTime = mBackBrush.PlayTime /
            mBackBrush.Frames.size();
        break;
    }
}

void CProgressBar::AddBrushFrame(EProgressBarImageType Type, float StartX, float StartY,
    float SizeX, float SizeY)
{
    AddBrushFrame(Type, FVector2D(StartX, StartY),
        FVector2D(SizeX, SizeY));
}

void CProgressBar::SetCurrentFrame(EProgressBarImageType Type, int Frame)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.Frame = Frame;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.Frame = Frame;
        break;
    }
}

void CProgressBar::SetAnimationPlayTime(EProgressBarImageType Type, float PlayTime)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.PlayTime = PlayTime;

        mBackBrush.FrameTime = mBackBrush.PlayTime /
            mBackBrush.Frames.size();
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.PlayTime = PlayTime;

        mFillBrush.FrameTime = mFillBrush.PlayTime /
            mFillBrush.Frames.size();
        break;
    }
}

void CProgressBar::SetAnimationPlayRate(EProgressBarImageType Type, float PlayRate)
{
    switch (Type)
    {
    case EProgressBarImageType::Back:
        mBackBrush.PlayRate = PlayRate;
        break;
    case EProgressBarImageType::Fill:
        mFillBrush.PlayRate = PlayRate;
        break;
    }
}

void CProgressBar::SetPercent(float Percent)
{
    mPercent = Percent;
}

void CProgressBar::SetBarDir(EProgressBarDir Dir)
{
    mBarDir = Dir;
}

bool CProgressBar::Init()
{
    CWidget::Init();

    return true;
}

void CProgressBar::Update(float DeltaTime)
{
    CWidget::Update(DeltaTime);
}

void CProgressBar::Render()
{
    CWidget::Render();

    RenderBrush(mBackBrush, mSize);

    FVector2D   Size = mSize;

    switch (mBarDir)
    {
    case EProgressBarDir::RightToLeft:
        Size.x *= mPercent;
        break;
    case EProgressBarDir::TopToBottom:
        Size.y *= mPercent;
        break;
    }

    RenderBrush(mFillBrush, Size);
}

void CProgressBar::Render(const FVector3D& Pos)
{
    CWidget::Render(Pos);

    FVector2D   RenderPos = mRenderPos;

    RenderPos.x += Pos.x;
    RenderPos.y += Pos.y;

    RenderBrush(mBackBrush, RenderPos, mSize);

    FVector2D   Size = mSize;

    switch (mBarDir)
    {
    case EProgressBarDir::RightToLeft:
        Size.x *= mPercent;
        break;
    case EProgressBarDir::TopToBottom:
        Size.y *= mPercent;
        break;
    }

    RenderBrush(mFillBrush, RenderPos, Size);
}

void CProgressBar::RenderBrush(const FUIBrush& Brush, 
    const FVector2D& Size)
{
    FMatrix matScale, matRot, matTranslate, matWorld;

    matScale.Scaling(Size);
    matRot.RotationZ(mRotation);
    matTranslate.Translation(mRenderPos);

    matWorld = matScale * matRot * matTranslate;

    mTransformCBuffer->SetWorldMatrix(matWorld);
    mTransformCBuffer->SetProjMatrix(mUIProj);
    mTransformCBuffer->SetPivot(mPivot);

    mTransformCBuffer->UpdateBuffer();

    //mUICBuffer->SetWidgetColor(mColor);

    mUICBuffer->SetTint(Brush.Tint);

    if (Brush.Texture)
    {
        mUICBuffer->SetTextureEnable(true);

        Brush.Texture->SetShader(0, EShaderBufferType::Pixel,
            0);
    }

    else
        mUICBuffer->SetTextureEnable(false);

    if (Brush.AnimationEnable)
    {
        mUICBuffer->SetAnimationEnable(true);

        int Frame = Brush.Frame;

        FAnimationFrame FrameInfo = Brush.Frames[Frame];
        mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
            FrameInfo.Start.x + FrameInfo.Size.x,
            FrameInfo.Start.y + FrameInfo.Size.y);
    }

    else
        mUICBuffer->SetAnimationEnable(false);


    mUICBuffer->UpdateBuffer();

    mShader->SetShader();

    mMesh->Render();
}

void CProgressBar::RenderBrush(const FUIBrush& Brush,
    const FVector2D& Pos, const FVector2D& Size)
{
    FMatrix matScale, matRot, matTranslate, matWorld;

    matScale.Scaling(Size);
    matRot.RotationZ(mRotation);
    matTranslate.Translation(Pos);

    matWorld = matScale * matRot * matTranslate;

    FMatrix matView = mScene->GetCameraManager()->GetViewMatrix();
    FMatrix matProj = mScene->GetCameraManager()->GetProjMatrix();

    mTransformCBuffer->SetWorldMatrix(matWorld);
    mTransformCBuffer->SetViewMatrix(matView);
    mTransformCBuffer->SetProjMatrix(matProj);
    mTransformCBuffer->SetPivot(mPivot);

    mTransformCBuffer->UpdateBuffer();

    //mUICBuffer->SetWidgetColor(mColor);

    mUICBuffer->SetTint(Brush.Tint);

    if (Brush.Texture)
    {
        mUICBuffer->SetTextureEnable(true);

        Brush.Texture->SetShader(0, EShaderBufferType::Pixel,
            0);
    }

    else
        mUICBuffer->SetTextureEnable(false);

    if (Brush.AnimationEnable)
    {
        mUICBuffer->SetAnimationEnable(true);

        int Frame = Brush.Frame;

        FAnimationFrame FrameInfo = Brush.Frames[Frame];
        mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
            FrameInfo.Start.x + FrameInfo.Size.x,
            FrameInfo.Start.y + FrameInfo.Size.y);
    }

    else
        mUICBuffer->SetAnimationEnable(false);


    mUICBuffer->UpdateBuffer();

    mShader->SetShader();

    mMesh->Render();
}
