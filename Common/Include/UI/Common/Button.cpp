#include "Button.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Texture/Texture.h"
#include "Asset/Texture/TextureManager.h"
#include "Asset/Sound/Sound.h"
#include "Asset/Sound/SoundManager.h"
#include "Asset/Mesh/Mesh.h"
#include "Shader/UICBuffer.h"
#include "Shader/TransformCBuffer.h"
#include "Shader/Shader.h"
#include "../../Scene/Input.h"

CButton::CButton()
{
}

CButton::~CButton()
{
}

void CButton::SetTexture(EButtonState::Type State, const std::string& Name)
{
    if (mScene)
        mBrush[State].Texture = mScene->GetAssetManager()->FindTexture(Name);

    else
        mBrush[State].Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
}

void CButton::SetTexture(EButtonState::Type State, 
    const std::string& Name, const TCHAR* FileName)
{
    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
            return;

        mBrush[State].Texture = mScene->GetAssetManager()->FindTexture(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
            return;

        mBrush[State].Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
    }
}

void CButton::SetTexture(EButtonState::Type State, CTexture* Texture)
{
    if (mScene)
        mBrush[State].Texture = Texture;

    else
        mBrush[State].Texture = Texture;
}

void CButton::SetTint(EButtonState::Type State, float r, float g, float b)
{
    mBrush[State].Tint.x = r;
    mBrush[State].Tint.y = g;
    mBrush[State].Tint.z = b;
}

void CButton::SetTint(EButtonState::Type State, FVector3D rgb)
{
    mBrush[State].Tint.x = rgb.x;
    mBrush[State].Tint.y = rgb.y;
    mBrush[State].Tint.z = rgb.z;
}

void CButton::SetOpacity(EButtonState::Type State, float Opacity)
{
    mBrush[State].Tint.w = Opacity;
}

void CButton::SetBrushAnimation(EButtonState::Type State, bool Animation)
{
    mBrush[State].AnimationEnable = Animation;
}

void CButton::AddBrushFrame(EButtonState::Type State, 
    const FVector2D& Start, const FVector2D& Size)
{
    FAnimationFrame Frame;
    Frame.Start = Start;
    Frame.Size = Size;

    mBrush[State].Frames.emplace_back(Frame);

    mBrush[State].FrameTime = mBrush[State].PlayTime /
        mBrush[State].Frames.size();
}

void CButton::AddBrushFrame(EButtonState::Type State, 
    float StartX, float StartY, float SizeX, float SizeY)
{
    FAnimationFrame Frame;
    Frame.Start.x = StartX;
    Frame.Start.y = StartY;
    Frame.Size.x = SizeX;
    Frame.Size.y = SizeY;

    mBrush[State].Frames.emplace_back(Frame);

    mBrush[State].FrameTime = mBrush[State].PlayTime /
        mBrush[State].Frames.size();
}

void CButton::SetCurrentFrame(EButtonState::Type State, int Frame)
{
    mBrush[State].Frame = Frame;
}

void CButton::SetAnimationPlayTime(EButtonState::Type State, float PlayTime)
{
    mBrush[State].PlayTime = PlayTime;

    mBrush[State].FrameTime = mBrush[State].PlayTime /
        mBrush[State].Frames.size();
}

void CButton::SetAnimationPlayRate(EButtonState::Type State, float PlayRate)
{
    mBrush[State].PlayRate = PlayRate;
}

void CButton::SetSound(EButtonEventState::Type State,
    const std::string& Name)
{
    if (mScene)
        mSound[State] = mScene->GetAssetManager()->FindSound(Name);

    else
        mSound[State] = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);

}

void CButton::SetSound(EButtonEventState::Type State, 
    const std::string& Name, const char* FileName)
{
    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadSound(Name, "UI", 
            false, FileName))
            return;

        mSound[State] = mScene->GetAssetManager()->FindSound(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetSoundManager()->LoadSound(Name, 
            "UI", false, FileName))
            return;

        mSound[State] = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
    }
}

bool CButton::Init()
{
    CWidget::Init();

    return true;
}

void CButton::Update(float DeltaTime)
{
    CWidget::Update(DeltaTime);

    if (mState != EButtonState::Disable)
    {
        if (mMouseOn)
        {
            if (mScene->GetInput()->GetMouseDown(EMouseButtonType::LButton))
            {
                mState = EButtonState::Click;
            }

            else if (mState == EButtonState::Click &&
                mScene->GetInput()->GetMouseUp(EMouseButtonType::LButton))
            {
                if (mSound[EButtonEventState::Click])
                    mSound[EButtonEventState::Click]->Play();

                if (mEventCallback[EButtonEventState::Click])
                    mEventCallback[EButtonEventState::Click]();

                mState = EButtonState::Hovered;
            }

            else if (mScene->GetInput()->GetMouseHold(EMouseButtonType::LButton))
            {
                mState = EButtonState::Click;
            }
        }
    }
}

void CButton::Render()
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

    mUICBuffer->SetTint(mBrush[mState].Tint);

    if (mBrush[mState].Texture)
    {
        mUICBuffer->SetTextureEnable(true);

        mBrush[mState].Texture->SetShader(0, EShaderBufferType::Pixel,
            0);
    }

    else
        mUICBuffer->SetTextureEnable(false);

    if (mBrush[mState].AnimationEnable)
    {
        mUICBuffer->SetAnimationEnable(true);

        int Frame = mBrush[mState].Frame;

        FAnimationFrame FrameInfo = mBrush[mState].Frames[Frame];
        mUICBuffer->SetUV(FrameInfo.Start.x, FrameInfo.Start.y,
            FrameInfo.Start.x + FrameInfo.Size.x,
            FrameInfo.Start.y + FrameInfo.Size.y);
    }

    else
        mUICBuffer->SetAnimationEnable(false);


    mUICBuffer->UpdateBuffer();

    mShader->SetShader();

    mMesh->Render();

    if (mChild)
        mChild->Render();
}

void CButton::Render(const FVector3D& Pos)
{
    CWidget::Render(Pos);

    if (mChild)
        mChild->Render(Pos);
}

void CButton::MouseHovered()
{
    if (mState == EButtonState::Normal)
    {
        if (mSound[EButtonEventState::Hovered])
            mSound[EButtonEventState::Hovered]->Play();

        if (mEventCallback[EButtonEventState::Hovered])
            mEventCallback[EButtonEventState::Hovered]();

        mState = EButtonState::Hovered;
    }
}

void CButton::MouseUnHovered()
{
    if (mState != EButtonState::Disable)
        mState = EButtonState::Normal;
}
