#pragma once

#include "../Widget.h"

namespace EButtonState
{
	enum Type
	{
		Normal,
		Hovered,
		Click,
		Disable,
		End
	};
}

namespace EButtonEventState
{
	enum Type
	{
		Hovered,
		Click,
		End
	};
}

class CButton :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CButton();
	virtual ~CButton();

protected:
	CSharedPtr<CWidget>	mChild;
	FUIBrush	mBrush[EButtonState::End];
	EButtonState::Type		mState = EButtonState::Normal;

	CSharedPtr<class CSound>	mSound[EButtonEventState::End];
	std::function<void()>		mEventCallback[EButtonEventState::End];

	FVector4D		mColor = FVector4D::White;

public:
	void ButtonEnable(bool Enable)
	{
		mState = Enable ? EButtonState::Normal : EButtonState::Disable;
	}

	void SetChild(CWidget* Child)
	{
		if (Child)
		{
			Child->SetParent(this);
			Child->SetSize(mSize);
		}

		mChild = Child;
	}

	virtual void SetSize(const FVector2D& Size)
	{
		CWidget::SetSize(Size);

		if (mChild)
			mChild->SetSize(mSize);
	}

	virtual void SetSize(float x, float y)
	{
		CWidget::SetSize(x, y);

		if (mChild)
			mChild->SetSize(mSize);
	}

	void SetColor(const FVector4D& Color)
	{
		mColor = Color;
	}

	void SetColor(float r, float g, float b, float a)
	{
		mColor = FVector4D(r, g, b, a);
	}

	void SetColor(float r, float g, float b)
	{
		mColor.x = r;
		mColor.y = g;
		mColor.z = b;
	}

	void SetOpacity(float Opacity)
	{
		mColor.w = Opacity;
	}


public:
	void SetTexture(EButtonState::Type State, const std::string& Name);
	void SetTexture(EButtonState::Type State, const std::string& Name,
		const TCHAR* FileName);
	void SetTexture(EButtonState::Type State, class CTexture* Texture);
	void SetTint(EButtonState::Type State, float r, float g, float b);
	void SetOpacity(EButtonState::Type State, float Opacity);
	void SetBrushAnimation(EButtonState::Type State, bool Animation);
	void AddBrushFrame(EButtonState::Type State,
		const FVector2D& Start, const FVector2D& Size);
	void AddBrushFrame(EButtonState::Type State,
		float StartX, float StartY, float SizeX, float SizeY);
	void SetCurrentFrame(EButtonState::Type State,
		int Frame);
	void SetAnimationPlayTime(EButtonState::Type State, float PlayTime);
	void SetAnimationPlayRate(EButtonState::Type State, float PlayRate);

	void SetSound(EButtonEventState::Type State, const std::string& Name);
	void SetSound(EButtonEventState::Type State, const std::string& Name,
		const char* FileName);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	virtual void MouseHovered();
	virtual void MouseUnHovered();

public:
	template <typename T>
	void SetEventCallback(EButtonEventState::Type State, 
		T* Obj, void(T::* Func)())
	{
		mEventCallback[State] = std::bind(Func, Obj);
	}
};

