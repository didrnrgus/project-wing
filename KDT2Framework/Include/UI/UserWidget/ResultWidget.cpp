#pragma once
#include "ResultWidget.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "UI/Common/TextBlock.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "Etc/DataStorageManager.h"
#include "Etc/JsonContainer.h"

CResultWidget::CResultWidget()
{
}

CResultWidget::~CResultWidget()
{
}

bool CResultWidget::Init()
{
	CSceneWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();
	mResolution = FVector2D(RS.Width, RS.Height);

	mResultTitle = mScene->GetUIManager()->CreateWidget<CTextBlock>("mResultTitle");
	AddWidget(mResultTitle);
	mResultTitle->SetPivot(FVector2D::One * 0.5f);
	mResultTitle->SetSize(FVector2D(500.0f, 100.0f));
	mResultTitle->SetPos(FVector2D(mResolution.x * 0.5f, mResolution.y * 0.9f));
	mResultTitle->SetText(L"RESULT");
	mResultTitle->SetTextColor(FVector4D::Green);
	mResultTitle->SetAlignH(ETextAlignH::Center);
	mResultTitle->SetFontSize(100.f);
	mResultTitle->SetShadowEnable(true);
	mResultTitle->SetShadowOffset(5.f, 5.f);
	mResultTitle->SetTextShadowColor(FVector4D::Gray30);

	InitMemu();
	InitUserRankPrint();

	mArrSubCategoryMunuTap.resize((int)EResultMenuTap::End);
	mCurSubCategoryMenuTap.resize((int)EResultMenuTap::End, 0);
	OnClickMainCategoryMenuTapButton(EResultMenuTap::Map);
	return true;
}

void CResultWidget::Update(float DeltaTime)
{
	CSceneWidget::Update(DeltaTime);


}

void CResultWidget::InitMemu()
{
	// 메인카테고리
	FVector2D _pivotMain = FVector2D(0.0f, 1.0f);
	FVector2D _sizeMain = FVector2D(200.0f, 50.0f);
	FVector2D _basePosMain = FVector2D(400.0f, mResolution.y * 0.8f); // 임의
	float _gapMainX = 10.0f;

	for (int i = 0; i < (int)EResultMenuTap::End; i++)
	{
		FMenuTap _menuTap;

		CSharedPtr<CTextBlock> _menuTapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_menuTapText");
		AddWidget(_menuTapText);
		_menuTapText->SetPivot(_pivotMain);
		_menuTapText->SetSize(_sizeMain);
		_menuTapText->SetPos(_basePosMain + FVector2D((_sizeMain.x + _gapMainX) * i, 0.0f));
		_menuTapText->SetText(mArrMenuTapText[i]);
		_menuTapText->SetTextColor(FVector4D::Green);
		_menuTapText->SetAlignH(ETextAlignH::Center);
		_menuTapText->SetFontSize(mMenuTextFontSize);
		_menuTapText->SetShadowEnable(true);
		_menuTapText->SetShadowOffset(3.f, 3.f);
		_menuTapText->SetTextShadowColor(FVector4D::Gray30);
		_menuTap.menuTapText = _menuTapText;

		CSharedPtr<CButton> _menuTapButton = mScene->GetUIManager()->CreateWidget<CButton>("_menuTapButton");
		AddWidget(_menuTapButton);
		_menuTapButton->SetPivot(_pivotMain);
		_menuTapButton->SetSize(_sizeMain);
		_menuTapButton->SetColor(FVector4D::Transparent);
		_menuTapButton->SetPos(_basePosMain + FVector2D((_sizeMain.x + _gapMainX) * i, 0.0f));
		_menuTapButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		_menuTapButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("OnClickMenuTapButton(): " + std::to_string(i));
				OnClickMainCategoryMenuTapButton(static_cast<EResultMenuTap::Type>(i));
			});
		_menuTap.menuTapButton = _menuTapButton;

		mArrCategoryMenuTap.push_back(_menuTap);
	}

	FVector2D _pivotSub = FVector2D(0.0f, 1.0f);
	FVector2D _sizeSub = FVector2D(180.0f, 50.0f);
	FVector2D _basePosSub = _basePosMain + FVector2D(0.0f, -(_sizeSub.y + 5.0f));
	float _gapSubX = 10.0f;

	// 서브카테고리, 맵
	//int _mapCount = CDataStorageManager::GetInst()->GetMapInfoCount();
	int _mapCount = 3;

	for (int i = 0; i < _mapCount; i++)
	{
		FMenuTap _menuTap;

		CSharedPtr<CTextBlock> _menuTapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_menuTapText");
		AddWidget(_menuTapText);
		_menuTapText->SetPivot(_pivotSub);
		_menuTapText->SetSize(_sizeSub);
		_menuTapText->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapText->SetText(L"abcde");
		_menuTapText->SetTextColor(FVector4D::Green);
		_menuTapText->SetAlignH(ETextAlignH::Center);
		_menuTapText->SetFontSize(mMenuTextFontSize);
		_menuTapText->SetShadowEnable(true);
		_menuTapText->SetShadowOffset(3.f, 3.f);
		_menuTapText->SetTextShadowColor(FVector4D::Gray30);
		_menuTap.menuTapText = _menuTapText;

		CSharedPtr<CButton> _menuTapButton = mScene->GetUIManager()->CreateWidget<CButton>("_menuTapButton");
		AddWidget(_menuTapButton);
		_menuTapButton->SetPivot(_pivotSub);
		_menuTapButton->SetSize(_sizeSub);
		_menuTapButton->SetColor(FVector4D::Transparent);
		_menuTapButton->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		_menuTapButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("OnClickSubCategoryMenuTapButton(): " + std::to_string(i));
				OnClickSubCategoryMenuTapButton(i);
			});
		_menuTap.menuTapButton = _menuTapButton;

		mArrSubCategoryMunuTap[(int)EResultMenuTap::Map].push_back(_menuTap);
		//mArrMapMenuTap.push_back(_menuTap);
	}

	// 서브카테고리, 캐릭터
	//int _characterCount = CDataStorageManager::GetInst()->GetCharacterCount();
	int _characterCount = 5;

	for (int i = 0; i < _characterCount; i++)
	{
		FMenuTap _menuTap;

		CSharedPtr<CTextBlock> _menuTapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_menuTapText");
		AddWidget(_menuTapText);
		_menuTapText->SetPivot(_pivotSub);
		_menuTapText->SetSize(_sizeSub);
		_menuTapText->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapText->SetText(L"abcde");
		_menuTapText->SetTextColor(FVector4D::Green);
		_menuTapText->SetAlignH(ETextAlignH::Center);
		_menuTapText->SetFontSize(mMenuTextFontSize);
		_menuTapText->SetShadowEnable(true);
		_menuTapText->SetShadowOffset(3.f, 3.f);
		_menuTapText->SetTextShadowColor(FVector4D::Gray30);
		_menuTap.menuTapText = _menuTapText;

		CSharedPtr<CButton> _menuTapButton = mScene->GetUIManager()->CreateWidget<CButton>("_menuTapButton");
		AddWidget(_menuTapButton);
		_menuTapButton->SetPivot(_pivotSub);
		_menuTapButton->SetSize(_sizeSub);
		_menuTapButton->SetColor(FVector4D::Transparent);
		_menuTapButton->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		_menuTapButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("OnClickSubCategoryMenuTapButton(): " + std::to_string(i));
				OnClickSubCategoryMenuTapButton(i);
			});
		_menuTap.menuTapButton = _menuTapButton;

		mArrSubCategoryMunuTap[(int)EResultMenuTap::Character].push_back(_menuTap);
		//mArrCharacterMenuTap.push_back(_menuTap);
	}
}

void CResultWidget::InitUserRankPrint()
{
	// mRankMaxCount 인원수에 맞게 미리 위치 세팅
	FVector2D _basePos = FVector2D(400.0f, mResolution.y * 0.63f); // 임의
	float _gapY = mRowTextFontSize;

	for (int i = 0; i < mRankMaxCount; i++)
	{
		FUserPrintGroup _group;
		SetPositionUserPrintRow(_group, _basePos + FVector2D(0.0f, -(30.0f + _gapY) * i));
	}
}

void CResultWidget::SetPositionUserPrintRow(FUserPrintGroup& _groupOut, FVector2D _basePos)
{
	// 여기서 Row 모양 세팅
	FVector2D _pivot = FVector2D(0.0f, 1.0f);
	FVector2D _sizeName = FVector2D(150.0f, 50.0f);
	FVector2D _sizeMap = FVector2D(150.0f, 50.0f);
	FVector2D _sizeCharacter = FVector2D(150.0f, 50.0f);
	FVector2D _sizeDistance = FVector2D(150.0f, 50.0f);

	float _gapX = 5.0f;

	FVector2D _posName = _basePos;
	FVector2D _posMap = _posName + FVector2D(_sizeName.x + _gapX, 0.0f);
	FVector2D _posCharacter = _posMap + FVector2D(_sizeMap.x + _gapX, 0.0f);
	FVector2D _posDistance = _posCharacter + FVector2D(_sizeCharacter.x + _gapX, 0.0f);

	FVector4D _color = FVector4D::White;

	CSharedPtr<CTextBlock> _nameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_nameText");
	AddWidget(_nameText);
	SetInfoTextBlock(_nameText, _pivot, _sizeName, _posName, _color, L"white");
	_groupOut.nameText = _nameText;

	CSharedPtr<CTextBlock> _mapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_mapText");
	AddWidget(_mapText);
	SetInfoTextBlock(_mapText, _pivot, _sizeMap, _posMap, _color, L"yellow");
	_groupOut.mapText = _mapText;

	CSharedPtr<CTextBlock> _characterText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_characterText");
	AddWidget(_characterText);
	SetInfoTextBlock(_characterText, _pivot, _sizeCharacter, _posCharacter, _color, L"jethro");
	_groupOut.characterText = _characterText;

	CSharedPtr<CTextBlock> _distanceText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_distanceText");
	AddWidget(_distanceText);
	SetInfoTextBlock(_distanceText, _pivot, _sizeDistance, _posDistance, _color, L"12331m");
	_groupOut.distanceText = _distanceText;

	FVector2D _pivotItem = FVector2D(0.5f, 0.5f);
	FVector2D _sizeItem = FVector2D(50.0f, 50.0f);
	FVector2D _posItemBase = _posDistance + FVector2D(_sizeDistance.x + _gapX, -_sizeItem.y * 0.5f);
	float _itemGapX = 5.0f;
	float _innerItemSizeRate = 0.65f;
	//int _itemCount = CDataStorageManager::GetInst()->GetSelectableItemCount();
	int _itemCount = 3;

	for (int i = 0; i < _itemCount; i++)
	{
		FVector2D _posItem = _posItemBase + FVector2D((_sizeItem.x + _itemGapX) * i, 0.0f);
		CSharedPtr<CImage> _itemSlotImage = mScene->GetUIManager()->CreateWidget<CImage>("_itemSlotImage");
		AddWidget(_itemSlotImage);
		SetImage(_itemSlotImage, _pivotItem, _sizeItem, _posItem, _color, ITEM_EMPTY_SQUARE_NAME, ITEM_EMPTY_SQUARE_PATH);

		CSharedPtr<CImage> _itemImage = mScene->GetUIManager()->CreateWidget<CImage>("_itemImage");
		AddWidget(_itemImage);
		SetImage(_itemImage, _pivotItem, _sizeItem * _innerItemSizeRate, _posItem, _color, ITEM_HP_ICON_NAME, ITEM_HP_ICON_PATH);
	}

}

void CResultWidget::SetInfoTextBlock(CTextBlock* _textBlock, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const wchar_t* _str)
{
	_textBlock->SetPivot(_pivot);
	_textBlock->SetSize(_size);
	_textBlock->SetPos(_pos);
	_textBlock->SetText(_str);
	_textBlock->SetTextColor(_color);
	_textBlock->SetAlignH(ETextAlignH::Left);
	_textBlock->SetFontSize(mRowTextFontSize);
	_textBlock->SetShadowEnable(true);
	_textBlock->SetShadowOffset(3.f, 3.f);
	_textBlock->SetTextShadowColor(FVector4D::Gray30);
}

void CResultWidget::SetImage(CImage* _image, FVector2D _pivot, FVector2D _size, FVector2D _pos, FVector4D _color, const char* _imageName, const wchar_t* _imagePath)
{
	_image->SetTexture(_imageName, _imagePath);
	_image->SetPivot(_pivot);
	_image->SetSize(_size);
	_image->SetPos(_pos);
	_image->SetColor(_color);
	_image->SetZOrder(ZORDER_LOBBY_MY_ITEM_ICON);
}

void CResultWidget::OnClickMainCategoryMenuTapButton(EResultMenuTap::Type _tap)
{
	mCurMainCategoryMenuTap = _tap;
	// 메뉴 모양 업데이트, 아래 리스트
	// 어차피 텍스트나 아이템 이미지 등 위치 정렬은 되어있으니까, 데이터 업데이트만 하면 됨.
	// 이것만 업뎃 std::vector<FUserPrintGroup> mArrUserInfoText;
	std::vector<FUserRankInfo> _arrRankInfo;

	if (mCurMainCategoryMenuTap == EResultMenuTap::Map)
	{
		_arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByMap(0);
	}
	else if (mCurMainCategoryMenuTap == EResultMenuTap::Character)
	{
		_arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByCharacter(0);
	}

	UpdateUserRankPrint(_arrRankInfo);



}

void CResultWidget::OnClickSubCategoryMenuTapButton(int _index)
{
	const int _menuTapIndex = (int)mCurMainCategoryMenuTap;
	mCurSubCategoryMenuTap[_menuTapIndex] = _index;

	// 서브 메뉴 탭 변경.
	// 일단 다끄고.
	for (auto& _arrMain : mArrSubCategoryMunuTap)
	{
		for (auto& _sub : _arrMain)
		{
			_sub.menuTapButton->SetEnable(false);
			_sub.menuTapText->SetEnable(false);
		}
	}

	{
		auto& _arrMain = mArrSubCategoryMunuTap[_menuTapIndex];
		for (auto& _sub : _arrMain)
		{
			_sub.menuTapButton->SetEnable(true);
			_sub.menuTapText->SetEnable(true);
		}
	}


	// 데이터 가져오기.
	std::vector<FUserRankInfo> _arrRankInfo;

	_arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(
		mCurMainCategoryMenuTap
		, mCurSubCategoryMenuTap[_menuTapIndex]
	);

	// Print Row.

}

void CResultWidget::UpdateUserRankPrint(std::vector<FUserRankInfo> _arrInfo)
{
	// 세팅해놓은 텍스트나 이미지의 데이터 업데이트.

}
