#pragma once
#include "UI\UserWidget\UserWidget.h"

class CDebugWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CDebugWidget();
	virtual ~CDebugWidget();

protected:
	CSharedPtr<class CTextBlock> mDebugText;

public:
	virtual bool Init();

	void SetDebugText(const wchar_t* wc);
	const wchar_t* GetDebugText();
};

