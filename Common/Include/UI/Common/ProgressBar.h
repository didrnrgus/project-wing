#pragma once

#include "../Widget.h"

enum class EProgressBarImageType
{
	Back,
	Fill
};

// 바가 작아지는 방향을 기준으로 이름을 정함.
enum class EProgressBarDir
{
	RightToLeft,
	TopToBottom
};

class CProgressBar :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CProgressBar();
	virtual ~CProgressBar();

protected:
	FUIBrush		mBackBrush;
	FUIBrush		mFillBrush;
	EProgressBarDir	mBarDir = EProgressBarDir::RightToLeft;
	float			mPercent = 1.f;

public:
	void SetTexture(EProgressBarImageType Type,
		const std::string& Name);
	void SetTexture(EProgressBarImageType Type,
		const std::string& Name,
		const TCHAR* FileName);
	void SetTexture(EProgressBarImageType Type, class CTexture* Texture);
	void SetTint(EProgressBarImageType Type, float r, float g, float b);
	void SetOpacity(EProgressBarImageType Type, float Opacity);
	void SetBrushAnimation(EProgressBarImageType Type, 
		bool Animation);
	void AddBrushFrame(EProgressBarImageType Type, 
		const FVector2D& Start, const FVector2D& Size);
	void AddBrushFrame(EProgressBarImageType Type, 
		float StartX, float StartY,
		float SizeX, float SizeY);
	void SetCurrentFrame(EProgressBarImageType Type, int Frame);
	void SetAnimationPlayTime(EProgressBarImageType Type, float PlayTime);
	void SetAnimationPlayRate(EProgressBarImageType Type, float PlayRate);
	void SetPercent(float Percent);
	void SetBarDir(EProgressBarDir Dir);


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);

protected:
	void RenderBrush(const FUIBrush& Brush,
		const FVector2D& Size);
	void RenderBrush(const FUIBrush& Brush,
		const FVector2D& Pos, const FVector2D& Size);
};

