#pragma once
#include "UI/UserWidget/SceneWidget.h"
#include "Etc/JsonContainer.h"

class CRankWidget : public CSceneWidget
{
	friend class CSceneUIManager;

protected:
	std::vector<const wchar_t*> mArrMenuTapText =
	{
		TEXT("All")
		,TEXT("Map")
		,TEXT("Character")
		,TEXT("End")
	};

protected:
	CRankWidget();
	virtual ~CRankWidget();

protected:

	struct FMenuTap
	{
		CSharedPtr<class CButton> menuTapButton;
		CSharedPtr<class CTextBlock> menuTapText;
	};
	std::vector<FMenuTap> mArrMainCategoryMenuTap;
	std::vector<std::vector<FMenuTap>> mArrSubCategoryMunuTap;

	struct FUserPrintGroup
	{
		CSharedPtr<class CTextBlock> nameText;
		CSharedPtr<class CTextBlock> mapText;
		CSharedPtr<class CTextBlock> characterText;
		CSharedPtr<class CTextBlock> distanceText;
		std::vector<CSharedPtr<class CImage>> arrSlotImage;
		std::vector<CSharedPtr<class CImage>> arrItemImage;
	};
	std::vector<FUserPrintGroup> mArrUserInfoText;

	CSharedPtr<class CTextBlock> mRankTitle; // Ÿ��Ʋ ǥ��

	const int mRankMaxCount = 10;
	const float mMenuTextFontSize = 30.0f;
	const float mRowTextFontSize = 30.0f;
	ERankMenuTap::Type mCurMainCategoryMenuTap = ERankMenuTap::Map;
	std::vector<int> mCurSubCategoryMenuTap;
	FVector2D mResolution;
	CSharedPtr<class CButton> mPrevButton;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

private:
	void InitCommonUI();
	void InitMemu(); // ����ī�װ�, ����ī�װ� -> �̱ۿ�
	void InitProperty(FVector2D _basePos);
	void InitUserRankPrint(FVector2D _basePos, int _count); // ���̺� �� ���˸� ����. 

	void SetPositionUserPrintRow(FUserPrintGroup& _groupOut, FVector2D _basePos); // ��ġ��Ű�� ����.
	void SetInfoTextBlock(class CTextBlock* _textBlock, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const wchar_t* _str);
	void SetImage(class CImage* _image, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const char* _imageName, const wchar_t* _imagePath);

	void OnClickMainCategoryMenuTapButton(ERankMenuTap::Type _tap);
	void OnClickSubCategoryMenuTapButton(int _index);

	void UpdateUserRankPrint(std::vector<FUserRankInfo> _arrInfo); // ������ ������Ʈ
	void SetButton(CButton& _button, const char* _name, const wchar_t* _path);

};

