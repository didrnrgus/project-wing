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

	// �����ư
	std::vector<CSharedPtr<class CButton>> mItemSlots;

	// ������ Ŭ�� �ϰ�, �ٲ� �̹���
	std::vector<CSharedPtr<class CImage>> mItemImages;

	// ������ ������ �������� ���� ���� ���η� �� ���´�
	// ������ ��ư���� �������� �ϳ� ����(Ŭ��) �ϸ� ����� 3��
	// ������� ���� �ȿ� ������ �������� ������ �ȴ�
	std::vector<CSharedPtr<class CButton>> mItemButtons; 
	std::vector<const wchar_t*> mItemImagePaths;

	FVector2D SlotPosBase;
	FVector2D SlotPosAdd;
	FVector2D SlotSize;
	float SlotInnerItemSizeRate;
	
	bool mIsHost;
	bool mIsMultiPlay;

	int itemTypeCount;
	int itemSlotCount;

	int curSelectedSlot;

public:
	virtual bool Init();

private:
	void CharacterLeftButtonClick();
	void CharacterRightButtonClick();
	void MapLeftButtonClick();
	void MapRightButtonClick();

	void ItemSlotClick(int Index);
	void ItemClick(int Index);

	void SelectItemForSlot(int _slotIndex, int _itemIndex);
	void TriggerItemButtons(int _index);
	void SetButton(class CButton& _button, const char* _name, const wchar_t* _path);
};

