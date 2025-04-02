#pragma once
#include "UI\UserWidget\UserWidget.h"

class CSceneWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CSceneWidget();
	virtual ~CSceneWidget();

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

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

	void SetEnableFPS(bool _isEnable);
	void ShowLoading(bool _isLoading);
	void AddQueueLoadingDescText(const std::wstring _wstrDesc, bool _isSkip);
	void UpdateLoading(float DeltaTime);
	bool IsLoading() { return mIsLoading; }
};

