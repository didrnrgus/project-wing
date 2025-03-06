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

	// ������ ������ �������� ���� ���� ���η� �� ���´�
	// ������ ��ư���� �������� �ϳ� ����(Ŭ��) �ϸ� ����� 3��
	// ������� ���� �ȿ� ������ �������� ������ �ȴ�
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

