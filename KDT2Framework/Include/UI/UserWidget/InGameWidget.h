#pragma once
#include "UI/UserWidget/SceneWidget.h"
#include "Interface/IGamePlayStateController.h"

class CInGameWidget : public CSceneWidget, public IGamePlayStateController
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

	struct FPlayerOtherStatInfo
	{
		int id;
		float maxHp;
		CSharedPtr<class CTextBlock> nameText;
		CSharedPtr<class CTextBlock> distText;
		CSharedPtr<class CImage> hpBackImage;
		CSharedPtr<class CImage> hpImage;
		std::vector<CSharedPtr<class CImage>> itemSlotImages;
		std::vector<CSharedPtr<class CImage>> itemImages;
	};

	std::vector<FPlayerOtherStatInfo> mPlayerOtherStatInfos;

	CSharedPtr<class CTextBlock> mStartCountText;
	std::vector<const wchar_t*> mStartCountTextArr;

	CSharedPtr<class CTextBlock> mPlayDistanceText;
	//int mPlayDistance;

	int mItemSlotCount;

	FVector2D mSlotPosBase;
	FVector2D mSlotPosAdd;
	FVector2D mSlotSize;
	float mSlotInnerItemSizeRate;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

	void SetStartCount(int count);
	int GetStartCountArrCount() { return mStartCountTextArr.size() - 1; }

private:
	void InitStartCountAndMyDistanceText();
	void InitTargetPlayerStat();
	void InitSelectedItemSlot();
	void InitOtherPlayersInfo();

	void UpdateTargetPlayerStat(float DeltaTime);
	void UpdateOtherPlayersInfo();
};

