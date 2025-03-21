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

	std::vector<CSharedPtr<class CImage>> mItemSlotImages;
	std::vector<CSharedPtr<class CImage>> mItemImages;

	std::vector<const wchar_t*> mItemImagePaths;
	std::vector<const wchar_t*> mSlotImagePaths;
	std::vector<const char*> mItemImageNames;
	std::vector<const char*> mSlotImageNames;

	int itemSlotCount;
	int itemTypeCount;

	FVector2D mSlotPosBase;
	FVector2D mSlotPosAdd;
	FVector2D mSlotSize;
	float mSlotInnerItemSizeRate;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

protected:
	void InitSelectedItemSlot();

	void UpdateTargetPlayerStat(float DeltaTime);
};

