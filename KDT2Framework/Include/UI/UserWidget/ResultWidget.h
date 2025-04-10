#pragma once
#include "UI/UserWidget/SceneWidget.h"
#include "Etc/JsonContainer.h"

class CResultWidget : public CSceneWidget
{
	friend class CSceneUIManager;

protected:
	std::vector<const wchar_t*> mArrMenuTapText =
	{
		TEXT("Map")
		,TEXT("Character")
		,TEXT("End")
	};

protected:
	CResultWidget();
	virtual ~CResultWidget();

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

	CSharedPtr<class CTextBlock> mResultTitle; // 타이틀 표시
	FUserPrintGroup mCurResultUserPrint; // 현재 플레이 한 유저.

	const int mRankMaxCount = 5;
	const float mMenuTextFontSize = 30.0f;
	const float mRowTextFontSize = 30.0f;
	EResultMenuTap::Type mCurMainCategoryMenuTap = EResultMenuTap::Map;
	std::vector<int> mCurSubCategoryMenuTap;
	FVector2D mResolution;
	CSharedPtr<class CButton> mNextButton;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

private:

	void InitMemu(); // 메인카테고리, 서브카테고리
	void InitProperty(FVector2D _basePos);
	void InitUserRankPrint();
	void InitMyResultScore();

	void SetPositionUserPrintRow(FUserPrintGroup& _groupOut, FVector2D _basePos); // 위치시키는 역할.
	void SetInfoTextBlock(class CTextBlock* _textBlock, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const wchar_t* _str);
	void SetImage(class CImage* _image, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const char* _imageName, const wchar_t* _imagePath);

	void OnClickMainCategoryMenuTapButton(EResultMenuTap::Type _tap);
	void OnClickSubCategoryMenuTapButton(int _index);

	void UpdateUserRankPrint(std::vector<FUserRankInfo> _arrInfo); // 데이터 업데이트
	void SetButton(CButton& _button, const char* _name, const wchar_t* _path);

};

