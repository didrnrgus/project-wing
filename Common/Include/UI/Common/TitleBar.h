#pragma once

#include "../Widget.h"

class CTitleBar :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CTitleBar();
	virtual ~CTitleBar();

protected:
	CWidget*		mUpdateWidget = nullptr;
	FUIBrush		mBrush;
	bool			mMouseHovered = false;
	bool			mMouseDrag = false;
	CSharedPtr<class CTextBlock>	mTextBlock;

public:
	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name,
		const TCHAR* FileName);
	void SetTexture(class CTexture* Texture);
	void SetTint(float r, float g, float b);
	void SetOpacity(float Opacity);
	void SetUpdateWidget(CWidget* Widget)
	{
		mUpdateWidget = Widget;
	}

	void SetText(const TCHAR* Text);

	virtual void SetSize(const FVector2D& Size);
	virtual void SetSize(float x, float y);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	virtual void MouseHovered();
	virtual void MouseUnHovered();
};

