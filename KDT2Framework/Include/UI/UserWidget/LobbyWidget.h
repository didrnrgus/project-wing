#pragma once
#include "UI\UserWidget\UserWidget.h"

class CLobbyWidget : public CUserWidget
{
	friend class CSceneUIManager;

protected:
	CLobbyWidget();
	virtual ~CLobbyWidget();

protected:
	CSharedPtr<class CButton>	mCharacterLeftButton;
	CSharedPtr<class CButton>	mCharacterRightButton;

	CSharedPtr<class CButton>	mMapLeftButton;
	CSharedPtr<class CButton>	mMapRightButton;

	std::vector<CSharedPtr<class CButton>>	mItemSlots;

	// 슬롯을 누르면 아이템이 윗쪽 방향 세로로 쭉 나온다
	// 아이템 버튼들이 나왔을때 하나 선택(클릭) 하면 사라짐 3개
	// 사라지면 슬롯 안에 선택한 아이템이 나오게 된다
	std::vector<CSharedPtr<class CButton>>	mItems; 
	
	bool mIsHost;
	bool mIsMultiPlay;

public:
	virtual bool Init();

private:
	void CharacterLeftButtonClick();
	void CharacterRightButtonClick();
	void MapLeftButtonClick();
	void MapRightButtonClick();

	void ItemSlotClick(int Index);
	void ItemClick(int Index);

	void SetButton(class CButton& _button, const char* _name, const wchar_t* _path);
};

