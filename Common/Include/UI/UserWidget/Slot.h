#pragma once

#include "UserWidget.h"

class CSlot :
    public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CSlot();
	virtual ~CSlot();

protected:
	CSharedPtr<class CImage>	mBack;
	CSharedPtr<class CImage>	mIcon;
	int				mIndex = 0;

public:
	void SetIndex(int Index)
	{
		mIndex = Index;
	}
	void SetTextureBack(const std::string& Name);
	void SetTextureBack(const std::string& Name,
		const TCHAR* FileName);
	void SetTextureBack(class CTexture* Texture);
	void SetTintBack(float r, float g, float b);
	void SetOpacityBack(float Opacity);
	void SetTextureIcon(const std::string& Name);
	void SetTextureIcon(const std::string& Name,
		const TCHAR* FileName);
	void SetTextureIcon(class CTexture* Texture);
	void SetTintIcon(float r, float g, float b);
	void SetOpacityIcon(float Opacity);

public:
	virtual void SetSize(const FVector2D& Size);
	virtual void SetSize(float x, float y);
	virtual void SetPos(const FVector2D& Pos);
	virtual void SetPos(float x, float y);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
};

