#include "Image.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/UICBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/Shader.h"

CImage::CImage()
{
}

CImage::~CImage()
{
}

void CImage::SetTexture(const std::string& Name)
{
    if (mScene)
        mBrush.Texture = mScene->GetAssetManager()->FindTexture(Name);

    else
        mBrush.Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
}

void CImage::SetTexture(const std::string& Name,
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

void CImage::SetTexture(CTexture* Texture)
{
    if (mScene)
        mBrush.Texture = Texture;

    else
        mBrush.Texture = Texture;
}

void CImage::SetTint(float r, float g, float b)
{
    mBrush.Tint.x = r;
    mBrush.Tint.y = g;
    mBrush.Tint.z = b;
}

void CImage::SetOpacity(float Opacity)
{
    mBrush.Tint.w = Opacity;
}

void CImage::SetBrushAnimation(bool Animation)
{
    mBrush.AnimationEnable = Animation;
}

void CImage::AddBrushFrame(const FVector2D& Start,
    const FVector2D& Size)
{
    FAnimationFrame Frame;
    Frame.Start = Start;
    Frame.Size = Size;

    mBrush.Frames.emplace_back(Frame);

    mBrush.FrameTime = mBrush.PlayTime /
        mBrush.Frames.size();
}

void CImage::AddBrushFrame(float StartX, float StartY,
    float SizeX, float SizeY)
{
    FAnimationFrame Frame;
    Frame.Start.x = StartX;
    Frame.Start.y = StartY;
    Frame.Size.x = SizeX;
    Frame.Size.y = SizeY;

    mBrush.Frames.emplace_back(Frame);

    mBrush.FrameTime = mBrush.PlayTime /
        mBrush.Frames.size();
}

void CImage::SetCurrentFrame(int Frame)
{
    mBrush.Frame = Frame;
}

void CImage::SetAnimationPlayTime(float PlayTime)
{
    mBrush.PlayTime = PlayTime;

    mBrush.FrameTime = mBrush.PlayTime /
        mBrush.Frames.size();
}

void CImage::SetAnimationPlayRate(float PlayRate)
{
    mBrush.PlayRate = PlayRate;
}

bool CImage::Init()
{
    CWidget::Init();

    return true;
}

void CImage::Update(float DeltaTime)
{
    CWidget::Update(DeltaTime);
}

void CImage::Render()
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
}

void CImage::Render(const FVector3D& Pos)
{
    CWidget::Render(Pos);
}
