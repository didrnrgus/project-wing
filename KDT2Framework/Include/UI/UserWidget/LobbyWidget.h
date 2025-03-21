#pragma once
#include "UI\UserWidget\UserWidget.h"





class CLobbyWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CLobbyWidget();
	virtual ~CLobbyWidget();

protected:
	// character select
	CSharedPtr<class CButton> mCharacterLeftButton;
	CSharedPtr<class CButton> mCharacterRightButton;

	// map select
	CSharedPtr<class CButton> mMapLeftButton;
	CSharedPtr<class CButton> mMapRightButton;
	CSharedPtr<class CImage> mMapDifficultyImage;

	// move scene
	CSharedPtr<class CButton> mNextButton;
	CSharedPtr<class CButton> mPrevButton;

	// tooltip
	CSharedPtr<class CImage> mToolTipShasdowImage;
	CSharedPtr<class CImage> mToolTipBack2Image;
	CSharedPtr<class CImage> mToolTipBackImage;
	std::vector<CSharedPtr<class CTextBlock>> mItemInfoNameText;
	std::vector<CSharedPtr<class CTextBlock>> mItemInfoValueText;

	// playerInfo
	std::vector<CSharedPtr<class CTextBlock>> mPlayerStatNameText;
	std::vector<CSharedPtr<class CTextBlock>> mPlayerStatValueText;

	// mapInfo
	std::vector<CSharedPtr<class CTextBlock>> mMapInfoNameText;
	std::vector<CSharedPtr<class CTextBlock>> mMapInfoValueText;

	// 투명버튼
	std::vector<CSharedPtr<class CButton>> mItemSlots;
	// 아이템 슬롯 이미지 + / empty
	std::vector<CSharedPtr<class CImage>> mItemSlotImages;
	// 아이템 클릭 하고, 바뀔 이미지
	std::vector<CSharedPtr<class CImage>> mItemImages;

	// 슬롯을 누르면 아이템이 윗쪽 방향 세로로 쭉 나온다
	// 아이템 버튼들이 나왔을때 하나 선택(클릭) 하면 사라짐 3개
	// 사라지면 슬롯 안에 선택한 아이템이 나오게 된다
	std::vector<CSharedPtr<class CButton>> mItemButtons; 
	std::vector<CSharedPtr<class CImage>> mItemButtonSlotImages;
	std::vector<const wchar_t*> mItemImagePaths;
	std::vector<const wchar_t*> mSlotImagePaths;
	std::vector<const wchar_t*> mMapDifficultyImagePaths;

	std::vector<FVector4D> mMapDifficultyImageColors;

	FVector2D mResolution;
	FVector2D mSlotPosBase;
	FVector2D mSlotPosAdd;
	FVector2D mSlotSize;
	FVector2D mMapDifficultyImagePos;

	float mSlotInnerItemSizeRate;
	float mMapDifficultySinAngle;

	bool mIsHost;
	bool mIsMultiPlay;

	int itemTypeCount; // 나중에 하자
	const int itemSlotCount = 3;

	int curSelectedSlot;
	int curPlayerGraphicIndex = 0;
	int curDifficultyIndex = 0;

	std::string mSlotTextureNamePrefix;
	std::string mItemTextureNamePrefix;
	std::string mSlotButtonNamePrefix;
	std::string mMapDifficultyImageNamePrefix;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
private:
	void InitScrollSelectButtons();
	void InitItemButtons();
	void InitDifficultiImage();
	void InitNextPrevButton();
	void InitPlayerStatText();
	void InitMapInfoText();
	void InitItemInfoTooltip();

	void OnCharacterLeftButtonClick();
	void OnCharacterRightButtonClick();
	void UpdatePlayerStatText();

	void OnMapLeftButtonClick();
	void OnMapRightButtonClick();
	void UpdateMapInfoText();

	void SelectItemForSlot(int _slotIndex, int _itemTypeIndex);
	void TriggerItemButtons(int _index);
	void TriggerItemTooltip(int _itemTypeIndex, FVector2D _pos = FVector2D::Zero); // 아이템타입의 인덱스.
	void SetButton(class CButton& _button, const char* _name, const wchar_t* _path);
};

