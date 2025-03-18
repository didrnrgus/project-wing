#pragma once
#include "UI\UserWidget\UserWidget.h"

class CInGameWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CInGameWidget();
	virtual ~CInGameWidget();

private:
	CSharedPtr<class CImage> mHpFrontImage;
	FVector2D mHpFrontSize;
	class IPlayerStatController* mPlayerStat;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

protected:
	void UpdateTargetPlayerStat(float DeltaTime);
};

