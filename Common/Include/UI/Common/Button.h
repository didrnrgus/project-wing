﻿#pragma once

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
		Unhoverd,
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

public:
	void ButtonEnable(bool Enable)
	{
		mState = Enable ? EButtonState::Normal : EButtonState::Disable;
	}

	bool GetIsButtonEnable()
	{
		return mState != EButtonState::Disable;
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


public:
	void SetTexture(EButtonState::Type State, const std::string& Name);
	void SetTexture(EButtonState::Type State, const std::string& Name,
		const TCHAR* FileName);
	void SetTexture(EButtonState::Type State, class CTexture* Texture);
	void SetTint(EButtonState::Type State, float r, float g, float b);
	void SetTint(EButtonState::Type State, FVector3D rgb);
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
	void SetEventCallback(EButtonEventState::Type State, T* Obj, void(T::* Func)())
	{
		mEventCallback[State] = std::bind(Func, Obj);
	}

	// 람다 넣으려고.
	using T = std::function<void()>;
	void SetEventCallback(EButtonEventState::Type State, T&& Func)
	{
		/*
		// 두번쓰면 앞에꺼 덮어씌워짐.
		mMultiPlayButton->SetEventCallback(EButtonEventState::Click,
			[]()
			{
				CLog::PrintLog("CTitleWidget::MultiPlayButtonClick()");
			}
		);
		*/
		mEventCallback[State] = std::move(Func);
	}
};

