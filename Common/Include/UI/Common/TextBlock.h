#pragma once

#include "../Widget.h"
#include "Asset/Font/Font.h"

enum class ETextAlignH
{
	Left,
	Center,
	Right
};

enum class ETextAlignV
{
	Top,
	Middle,
	Bottom
};

class CTextBlock :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CTextBlock();
	virtual ~CTextBlock();

protected:
	ID2D1RenderTarget* mTarget = nullptr;
	std::wstring		mText;
	CSharedPtr<CFont>	mFont;
	IDWriteTextLayout* mLayout = nullptr;
	ID2D1SolidColorBrush* mTextColor = nullptr;
	bool				mTransparency = false;
	float				mOpacity = 1.f;

	bool				mShadow = false;
	ID2D1SolidColorBrush* mTextShadowColor = nullptr;
	bool				mShadowTransparency = false;
	float				mShadowOpacity = 1.f;
	FVector2D			mShadowOffset = { 1.f, 1.f };

	float				mFontSize = 20.f;
	ETextAlignH			mAlignH = ETextAlignH::Left;
	ETextAlignV			mAlignV = ETextAlignV::Middle;

public:
	const TCHAR* GetText()	const
	{
		return mText.c_str();
	}

	int GetTextCount()	const
	{
		return (int)mText.length();
	}

	virtual void SetSize(const FVector2D& Size)
	{
		CWidget::SetSize(Size);

		CreateTextLayout();
	}

	virtual void SetSize(float x, float y)
	{
		CWidget::SetSize(x, y);

		CreateTextLayout();
	}

public:
	void SetFont(const std::string& FontName);
	void SetText(const TCHAR* Text);
	void SetTextFromInt(int Number);
	void SetTextFromFloat(float Number);
	void AddText(const TCHAR* Text);
	void AddTextFromInt(int Number);
	void AddTextFromFloat(float Number);
	void PopBack();
	void ClearText();
	void EnableTransparency(bool Transparency);
	void SetTextColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a);
	void SetTextColor(const FVector4D& Color);
	void SetOpacity(float Opacity);
	void SetFontSize(float Size);
	void SetAlignH(ETextAlignH Align);
	void SetAlignV(ETextAlignV Align);
	void SetShadowEnable(bool Shadow);
	void SetShadowTransparent(bool Transparency);
	void SetTextShadowColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a);
	void SetTextShadowColor(const FVector4D& Color);
	void SetShadowOpacity(float Opacity);
	void SetShadowOffset(const FVector2D& Offset);
	void SetShadowOffset(float x, float y);

private:
	void CreateTextLayout();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	virtual bool CollisionMouse(CWidget** Result, const FVector2D& MousePos);
};

