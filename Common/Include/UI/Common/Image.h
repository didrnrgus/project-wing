#pragma once

#include "../Widget.h"

class CImage :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CImage();
	virtual ~CImage();

protected:
	FUIBrush		mBrush;

public:
	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name,
		const TCHAR* FileName);
	void SetTexture(class CTexture* Texture);
	void SetTint(float r, float g, float b);
	void SetOpacity(float Opacity);
	void SetBrushAnimation(bool Animation);
	void AddBrushFrame(const FVector2D& Start, const FVector2D& Size);
	void AddBrushFrame(float StartX, float StartY, 
		float SizeX, float SizeY);
	void SetCurrentFrame(int Frame);
	void SetAnimationPlayTime(float PlayTime);
	void SetAnimationPlayRate(float PlayRate);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
};

