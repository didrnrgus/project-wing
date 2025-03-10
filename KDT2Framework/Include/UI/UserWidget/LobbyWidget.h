#pragma once
#include "UI\UserWidget\UserWidget.h"

class CLobbyWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CLobbyWidget();
	virtual ~CLobbyWidget();

protected:
	CSharedPtr<class CButton> mCharacterLeftButton;
	CSharedPtr<class CButton> mCharacterRightButton;

	CSharedPtr<class CButton> mMapLeftButton;
	CSharedPtr<class CButton> mMapRightButton;

	CSharedPtr<class CImage> mMapDifficultyImage;

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

	FVector2D mSlotPosBase;
	FVector2D mSlotPosAdd;
	FVector2D mSlotSize;
	FVector2D mMapDifficultyImagePos;

	float mSlotInnerItemSizeRate;
	float mMapDifficultySinAngle;

	bool mIsHost;
	bool mIsMultiPlay;

	int itemTypeCount;
	int itemSlotCount;

	int curSelectedSlot;
	int curPlayerGraphicIndex;
	int curDifficulty;
	int maxDifficulty;

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

	void OnCharacterLeftButtonClick();
	void OnCharacterRightButtonClick();
	void OnMapLeftButtonClick();
	void OnMapRightButtonClick();

	void SelectItemForSlot(int _slotIndex, int _itemIndex);
	void TriggerItemButtons(int _index);
	void SetButton(class CButton& _button, const char* _name, const wchar_t* _path);
};

