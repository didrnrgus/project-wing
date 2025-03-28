#pragma once

#include "../Widget.h"

class CUserWidget :
    public CWidget
{
	friend class CSceneUIManager;

protected:
	CUserWidget();
	virtual ~CUserWidget();

private:
	CSharedPtr<class CImage> mLoadingBackImage;
	CSharedPtr<class CTextBlock> mLoadingText;
	CSharedPtr<class CTextBlock> mLoadingDescText;
	std::vector<const wchar_t*> mLoadingTextStrings;
	bool mIsLoading = false;
	float loadingUpdateTime = 0.0f;
	float loadingTextUpdateTime = 0.0f;
	bool mIsSkipLoadingTextUpdate = false;
	int curLoadingTextIndex = 0;
	std::list<std::wstring> mLoadingTextQueue;
	std::mutex mQueueMutex;

	CSharedPtr<class CTextBlock> mFpsText;

protected:
	std::vector<CSharedPtr<CWidget>>	mWidgetList;
	
protected:
	void SetIsSkipLoadingTextUpdate(bool _isSkip) { mIsSkipLoadingTextUpdate = _isSkip; }

public:
	void AddWidget(CWidget* Widget)
	{
		Widget->SetParent(this);
		mWidgetList.emplace_back(Widget);
	}

	void ShowLoading(bool isLoading);
	void AddQueueLoadingDescText(const std::wstring wstrDesc);
	void UpdateLoading(float DeltaTime);
	bool IsLoading() { return mIsLoading; }

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render();
	virtual void Render(const FVector3D& Pos);
	virtual bool CollisionMouse(CWidget** Result, const FVector2D& MousePos);

private:
	static bool SortCollision(const CSharedPtr<CWidget>& Src,
		const CSharedPtr<CWidget>& Dest);

	static bool SortRender(const CSharedPtr<CWidget>& Src,
		const CSharedPtr<CWidget>& Dest);
};

