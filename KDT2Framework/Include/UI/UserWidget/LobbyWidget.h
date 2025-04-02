#pragma once
#include "UI/UserWidget/SceneWidget.h"
#include "Interface/IObjectNetworkController.h"

class CLobbyWidget : public CSceneWidget, public IObjectNetworkController
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
	std::vector<CSharedPtr<class CTextBlock>> mArrItemInfoNameText;
	std::vector<CSharedPtr<class CTextBlock>> mArrItemInfoValueText;

	// playerInfo
	std::vector<CSharedPtr<class CTextBlock>> mArrPlayerStatNameText;
	std::vector<CSharedPtr<class CTextBlock>> mArrPlayerStatValueText;

	// mapInfo
	std::vector<CSharedPtr<class CTextBlock>> mArrMapInfoNameText;
	std::vector<CSharedPtr<class CTextBlock>> mArrMapInfoValueText;

	// 투명버튼
	std::vector<CSharedPtr<class CButton>> mArrItemSlotButton;
	// 아이템 슬롯 이미지 + / empty
	std::vector<CSharedPtr<class CImage>> mArrItemSlotImage;
	// 아이템 클릭 하고, 바뀔 이미지
	std::vector<CSharedPtr<class CImage>> mArrItemImageInSlot;

	// player list -> 깔끔하게 구조체로
	struct PlayerWidgetGroup
	{
		CSharedPtr<class CImage> mPlayerHostImage;
		CSharedPtr<class CImage> mPlayerCheckBackImage;
		CSharedPtr<class CTextBlock> mPlayerText;
		std::vector<CSharedPtr<class CImage>> mArrPlayerSlotImage;
		std::vector<CSharedPtr<class CImage>> mArrPlayerItemImage;
	};
	std::vector<PlayerWidgetGroup> mArrPlayerWidgetGroup;

	// 슬롯을 누르면 아이템이 윗쪽 방향 세로로 쭉 나온다
	// 아이템 버튼들이 나왔을때 하나 선택(클릭) 하면 사라짐 3개
	// 사라지면 슬롯 안에 선택한 아이템이 나오게 된다
	std::vector<CSharedPtr<class CButton>> mArrItemButtonInList; 
	std::vector<CSharedPtr<class CImage>> mArrSlotImageInList;
	std::vector<CSharedPtr<class CImage>> mArrSlotFrameImageInList;

	std::vector<const char*> mArrItemImageName;
	std::vector<const wchar_t*> mArrItemImagePath;

	std::vector<const char*> mArrSlotImageName;
	std::vector<const wchar_t*> mArrSlotImagePath;

	std::vector<const char*> mArrMapDifficultyImageName;
	std::vector<const wchar_t*> mArrMapDifficultyImagePath;

	std::vector<FVector4D> mArrMapDifficultyImageColor;

	FVector2D mResolution;
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
	int curPlayerGraphicIndex = 0;
	int curDifficultyIndex = 0;

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
	void InitOtherPlayersInfo();

	void GoToTitle();
	void StartGame();

	void OnCharacterLeftButtonClick();
	void OnCharacterRightButtonClick();
	void UpdatePlayerStat();

	void OnMapLeftButtonClick();
	void OnMapRightButtonClick();
	void UpdateMapInfo();

	void SelectItemForSlot(int _slotIndex, int _itemTypeIndex);
	void TriggerItemArrayButtons(int _itemSlotIndex);
	void TriggerItemTooltip(int _itemTypeIndex, FVector2D _pos = FVector2D::Zero); // 아이템타입의 인덱스.
	void SetButton(class CButton& _button, const char* _name, const wchar_t* _path);

	void UpdateOtherPlayerInfo();

	// IObjectNetworkController을(를) 통해 상속됨
	void AddListener() override;
	void RemoveListener() override;
	void ProcessMessage(const RecvMessage& msg) override;
};

