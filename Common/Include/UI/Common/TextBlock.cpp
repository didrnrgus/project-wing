#include "TextBlock.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneAssetManager.h"
#include "Asset/AssetManager.h"
#include "Asset/Font/Font.h"
#include "Asset/Font/FontManager.h"
#include "../../Device.h"
#include "../../Scene/CameraManager.h"

CTextBlock::CTextBlock()
{
}

CTextBlock::~CTextBlock()
{
    SAFE_RELEASE(mLayout);
}

void CTextBlock::SetFont(const std::string& FontName)
{
    if (mScene)
        mFont = mScene->GetAssetManager()->FindFont(FontName);

    else
        mFont = CAssetManager::GetInst()->GetFontManager()->FindFont(FontName);

    CreateTextLayout();
}

void CTextBlock::SetText(const TCHAR* Text)
{
    mText = Text;

    CreateTextLayout();
}

void CTextBlock::SetTextFromInt(int Number)
{
    mText = std::to_wstring(Number);

    CreateTextLayout();
}

void CTextBlock::SetTextFromFloat(float Number)
{
    mText = std::to_wstring(Number);

    CreateTextLayout();
}

void CTextBlock::AddText(const TCHAR* Text)
{
    mText += Text;

    CreateTextLayout();
}

void CTextBlock::AddTextFromInt(int Number)
{
    mText += std::to_wstring(Number);

    CreateTextLayout();
}

void CTextBlock::AddTextFromFloat(float Number)
{
    mText += std::to_wstring(Number);

    CreateTextLayout();
}

void CTextBlock::PopBack()
{
    mText.pop_back();

    CreateTextLayout();
}

void CTextBlock::ClearText()
{
    mText.clear();

    CreateTextLayout();
}

void CTextBlock::EnableTransparency(bool Transparency)
{
    mTransparency = Transparency;
}

void CTextBlock::SetTextColor(unsigned char r, unsigned char g, 
    unsigned char b, unsigned char a)
{
    mTextColor = CAssetManager::GetInst()->GetFontManager()->FindFontColor(r, g, b, a);
}

void CTextBlock::SetTextColor(const FVector4D& Color)
{
    mTextColor = CAssetManager::GetInst()->GetFontManager()->FindFontColor(Color);
}

void CTextBlock::SetOpacity(float Opacity)
{
    mOpacity = Opacity;
}

void CTextBlock::SetFontSize(float Size)
{
    mFontSize = Size;

    if (!mLayout)
        return;

    DWRITE_TEXT_RANGE   Range = {};
    Range.startPosition = 0;
    Range.length = (UINT32)mText.length();

    mLayout->SetFontSize(mFontSize, Range);
}

void CTextBlock::SetAlignH(ETextAlignH Align)
{
    mAlignH = Align;

    if (!mLayout)
        return;

    switch (mAlignH)
    {
    case ETextAlignH::Left:
        mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        break;
    case ETextAlignH::Center:
        mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        break;
    case ETextAlignH::Right:
        mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
        break;
    }
}

void CTextBlock::SetAlignV(ETextAlignV Align)
{
    mAlignV = Align;

    if (!mLayout)
        return;

    switch (mAlignV)
    {
    case ETextAlignV::Top:
        mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        break;
    case ETextAlignV::Middle:
        mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        break;
    case ETextAlignV::Bottom:
        mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
        break;
    }
}

void CTextBlock::SetShadowEnable(bool Shadow)
{
    mShadow = Shadow;
}

void CTextBlock::SetShadowTransparent(bool Transparency)
{
    mShadowTransparency = Transparency;
}

void CTextBlock::SetTextShadowColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mTextShadowColor = CAssetManager::GetInst()->GetFontManager()->FindFontColor(r, g, b, a);
}

void CTextBlock::SetTextShadowColor(const FVector4D& Color)
{
    mTextShadowColor = CAssetManager::GetInst()->GetFontManager()->FindFontColor(Color);
}

void CTextBlock::SetShadowOpacity(float Opacity)
{
    mShadowOpacity = Opacity;
}

void CTextBlock::SetShadowOffset(const FVector2D& Offset)
{
    mShadowOffset = Offset;
}

void CTextBlock::SetShadowOffset(float x, float y)
{
    mShadowOffset.x = x;
    mShadowOffset.y = y;
}

void CTextBlock::CreateTextLayout()
{
    SAFE_RELEASE(mLayout);

    mLayout = mFont->CreateLayout(mText.c_str(), (int)mText.length(),
        mSize.x, mSize.y);

    if (!mLayout)
        return;

    DWRITE_TEXT_RANGE   Range = {};
    Range.startPosition = 0;
    Range.length = (UINT32)mText.length();

    mLayout->SetFontSize(mFontSize, Range);

    switch (mAlignH)
    {
    case ETextAlignH::Left:
        mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        break;
    case ETextAlignH::Center:
        mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        break;
    case ETextAlignH::Right:
        mLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
        break;
    }

    switch (mAlignV)
    {
    case ETextAlignV::Top:
        mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        break;
    case ETextAlignV::Middle:
        mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        break;
    case ETextAlignV::Bottom:
        mLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
        break;
    }
}

bool CTextBlock::Init()
{
    CWidget::Init();

    mTarget = CDevice::GetInst()->Get2DTarget();

    SetFont("Default");
    SetTextColor(FVector4D::White);
    SetTextShadowColor(FVector4D::Black);

    CreateTextLayout();

    return true;
}

void CTextBlock::Update(float DeltaTime)
{
    CWidget::Update(DeltaTime);
}

void CTextBlock::Render()
{
    CWidget::Render();

    mTarget->BeginDraw();

    FResolution RS = CDevice::GetInst()->GetResolution();

    D2D1_POINT_2F   Point;

    Point.x = mRenderPos.x;
    Point.y = RS.Height - mRenderPos.y - mSize.y;

    if (mShadow)
    {
        D2D1_POINT_2F   ShadowPoint = Point;

        ShadowPoint.x += mShadowOffset.x;
        ShadowPoint.y += mShadowOffset.y;

        if (mShadowTransparency)
            mTextColor->SetOpacity(mShadowOpacity);

        else
            mTextColor->SetOpacity(1.f);

        mTarget->DrawTextLayout(ShadowPoint, mLayout, mTextShadowColor,
            D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }

    if (mTransparency)
        mTextColor->SetOpacity(mOpacity);

    else
        mTextColor->SetOpacity(1.f);

    mTarget->DrawTextLayout(Point, mLayout, mTextColor,
        D2D1_DRAW_TEXT_OPTIONS_CLIP);

    mTarget->EndDraw();
}

void CTextBlock::Render(const FVector3D& Pos)
{
    CWidget::Render(Pos);

    mTarget->BeginDraw();

    FVector2D   RenderPos = mRenderPos;

    RenderPos.x += Pos.x;
    RenderPos.y += Pos.y;

    // 월드위치를 카메라를 기준으로 한 위치로 변경한다.
    FResolution RS = CDevice::GetInst()->GetResolution();

    const FVector3D& CameraPos =
        mScene->GetCameraManager()->GetCameraWorldPos();

    RenderPos.x -= CameraPos.x;
    RenderPos.y -= CameraPos.y;

    RenderPos.x += (RS.Width * 0.5f);
    RenderPos.y += (RS.Height * 0.5f);

    D2D1_POINT_2F   Point;

    Point.x = RenderPos.x;
    Point.y = RS.Height - RenderPos.y - mSize.y;

    if (mShadow)
    {
        D2D1_POINT_2F   ShadowPoint = Point;

        ShadowPoint.x += mShadowOffset.x;
        ShadowPoint.y += mShadowOffset.y;

        if (mShadowTransparency)
            mTextColor->SetOpacity(mShadowOpacity);

        else
            mTextColor->SetOpacity(1.f);

        mTarget->DrawTextLayout(ShadowPoint, mLayout, mTextShadowColor,
            D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }

    if (mTransparency)
        mTextColor->SetOpacity(mOpacity);

    else
        mTextColor->SetOpacity(1.f);

    mTarget->DrawTextLayout(Point, mLayout, mTextColor,
        D2D1_DRAW_TEXT_OPTIONS_CLIP);

    mTarget->EndDraw();
}

bool CTextBlock::CollisionMouse(CWidget** Result, 
    const FVector2D& MousePos)
{
    return false;
}
