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
#include "Etc/TaskManager.h"

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

	mArrSubCategoryMunuTap.resize((int)ERankMenuTap::End);
	mCurSubCategoryMenuTap.resize((int)ERankMenuTap::End, 0);

	// 공통
	InitCommonUI();

	// 내 기록.
	InitProperty(FVector2D(120.0f, 640.0f));
	InitMyResultScore();

	if (CNetworkManager::GetInst()->IsMultiplay())
	{
		// 멀티
		InitProperty(FVector2D(120.0f, 500.0f));

		int _playerCount = CMultiplayManager::GetInst()->GetPlayerCount();
		InitUserRankPrint(FVector2D(120.0f, 450.0f), _playerCount);

		std::vector<FUserRankInfo> _arrInfo;

		for (int i = 0; i < _playerCount; i++)
		{
			FUserRankInfo _rankInfo;
			auto _info = CMultiplayManager::GetInst()->GetPlayerInfoByIndex(i);
			_rankInfo.MultiPlayId = _info.id;
			_rankInfo.Name = _info.nickname;
			_rankInfo.Map = CDataStorageManager::GetInst()->GetSelectedMapIndex();
			_rankInfo.Character = _info.characterType;
			_rankInfo.Distance = _info.distance;
			_rankInfo.Item_0 = _info.arrItemType[0];
			_rankInfo.Item_1 = _info.arrItemType[1];
			_rankInfo.Item_2 = _info.arrItemType[2];
			_arrInfo.push_back(_rankInfo);
		}

		std::sort(_arrInfo.begin(), _arrInfo.end()
			, [](const FUserRankInfo& a, const FUserRankInfo& b) -> bool
			{
				return a.Distance > b.Distance; // 내림차순
			});

		UpdateUserRankPrint(_arrInfo);
	}
	else
	{
		// 싱글
		InitMemu();
		InitProperty(FVector2D(120.0f, 410.0f));
		InitUserRankPrint(FVector2D(120.0f, mResolution.y * 0.5f), mRankMaxCount);

		OnClickMainCategoryMenuTapButton(ERankMenuTap::Map);
		OnClickSubCategoryMenuTapButton(0);
	}

	return true;
}

void CResultWidget::InitCommonUI()
{
	mResultTitle = mScene->GetUIManager()->CreateWidget<CTextBlock>("mResultTitle");
	AddWidget(mResultTitle);
	mResultTitle->SetPivot(FVector2D::One * 0.5f);
	mResultTitle->SetSize(FVector2D(500.0f, 100.0f));
	mResultTitle->SetPos(FVector2D(mResolution.x * 0.5f, mResolution.y * 0.95f));
	mResultTitle->SetText(L"RESULT");
	mResultTitle->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
	mResultTitle->SetAlignH(ETextAlignH::Center);
	mResultTitle->SetFontSize(60.0f);
	mResultTitle->SetShadowEnable(true);
	mResultTitle->SetShadowOffset(5.f, 5.f);
	mResultTitle->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_SlateGray));

	mNewRecordText = mScene->GetUIManager()->CreateWidget<CTextBlock>("mNewRecordText");
	AddWidget(mNewRecordText);
	mNewRecordText->SetPivot(FVector2D(0.0f, 1.0f));
	mNewRecordText->SetSize(FVector2D(200.0f, 30.0f));
	mNewRecordText->SetPos(FVector2D(120.0f, 660.0f));
	mNewRecordText->SetText(L"[NEW RECORD!!]");
	mNewRecordText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
	mNewRecordText->SetAlignH(ETextAlignH::Left);
	mNewRecordText->SetAlignV(ETextAlignV::Top);
	mNewRecordText->SetFontSize(20);
	mNewRecordText->SetShadowEnable(true);
	mNewRecordText->SetShadowOffset(3.f, 3.f);
	mNewRecordText->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_SlateGray));
	bool _isNewRecord =
		CDataStorageManager::GetInst()->GetIsNewRecord()
		&& !CNetworkManager::GetInst()->IsMultiplay();
	mNewRecordText->SetEnable(_isNewRecord);

	mNextButton = mScene->GetUIManager()->CreateWidget<CButton>(ARROW_SQUARE_RIGHT_NAME);
	AddWidget(mNextButton);
	SetButton(*mNextButton.Get(), ARROW_SQUARE_RIGHT_NAME, ARROW_SQUARE_RIGHT_PATH);
	mNextButton->SetPivot(FVector2D::One * 0.5f);
	mNextButton->SetSize(FVector2D::One * 128 * 0.7f);
	mNextButton->SetPos(FVector2D(1200.0f, 660.0f));
	mNextButton->SetEventCallback(EButtonEventState::Click
		, [this]()
		{
			if(CNetworkManager::GetInst()->IsMultiplay())
				CMultiplayManager::GetInst()->ResetPlayerAfterInGame();

			CDataStorageManager::GetInst()->ClearCurUserResult();
			LoadScene(EGameScene::Lobby);
		});
}

void CResultWidget::InitMemu()
{
	// 메인카테고리
	FVector2D _pivotMain = FVector2D(0.0f, 1.0f);
	FVector2D _sizeMain = FVector2D(200.0f, mMenuTextFontSize + 10.0f);
	FVector2D _basePosMain = FVector2D(180.0f, mResolution.y * 0.7f);
	float _gapMainX = 10.0f;

	for (int i = 0; i < (int)ERankMenuTap::End; i++)
	{
		FMenuTap _menuTap;

		CSharedPtr<CTextBlock> _menuTapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_menuTapText");
		AddWidget(_menuTapText);
		_menuTapText->SetPivot(_pivotMain);
		_menuTapText->SetSize(_sizeMain);
		_menuTapText->SetPos(_basePosMain + FVector2D((_sizeMain.x + _gapMainX) * i, 0.0f));
		_menuTapText->SetText(ToUpperWString(mArrMenuTapText[i]).c_str());
		_menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		_menuTapText->SetAlignH(ETextAlignH::Center);
		_menuTapText->SetFontSize(mMenuTextFontSize);
		_menuTapText->SetZOrder(ZORDER_LOBBY_MY_ITEM_ICON);
		_menuTap.menuTapText = _menuTapText;

		CSharedPtr<CButton> _menuTapButton = mScene->GetUIManager()->CreateWidget<CButton>("_menuTapButton");
		AddWidget(_menuTapButton);
		_menuTapButton->SetPivot(_pivotMain);
		_menuTapButton->SetSize(_sizeMain);
		_menuTapButton->SetColor(FVector4D::GetColorFromString(COLOR_Transparent));
		_menuTapButton->SetPos(_basePosMain + FVector2D((_sizeMain.x + _gapMainX) * i, 0.0f));
		_menuTapButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		_menuTapButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("OnClickMenuTapButton(): " + std::to_string(i));
				OnClickMainCategoryMenuTapButton(static_cast<ERankMenuTap::Type>(i));
			});
		_menuTap.menuTapButton = _menuTapButton;

		mArrMainCategoryMenuTap.push_back(_menuTap);
	}

	FVector2D _pivotSub = FVector2D(0.0f, 1.0f);
	FVector2D _sizeSub = FVector2D(180.0f, _sizeMain.y + 5.0f);
	FVector2D _basePosSub = _basePosMain + FVector2D(0.0f, -(_sizeSub.y + 5.0f));
	float _gapSubX = 10.0f;

	// 서브카테고리, 맵
	int _mapCount = CDataStorageManager::GetInst()->GetMapInfoCount();

	for (int i = 0; i < _mapCount; i++)
	{
		FMenuTap _menuTap;
		auto _mapInfo = CDataStorageManager::GetInst()->GetMapInfo(i);

		CSharedPtr<CTextBlock> _menuTapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_menuTapText");
		AddWidget(_menuTapText);
		_menuTapText->SetPivot(_pivotSub);
		_menuTapText->SetSize(_sizeSub);
		_menuTapText->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapText->SetText(ToUpperWString(_mapInfo.Name).c_str());
		_menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		_menuTapText->SetAlignH(ETextAlignH::Center);
		_menuTapText->SetFontSize(mMenuTextFontSize - 5.0f);
		_menuTapText->SetZOrder(ZORDER_LOBBY_MY_ITEM_ICON);
		_menuTap.menuTapText = _menuTapText;

		CSharedPtr<CButton> _menuTapButton = mScene->GetUIManager()->CreateWidget<CButton>("_menuTapButton");
		AddWidget(_menuTapButton);
		_menuTapButton->SetPivot(_pivotSub);
		_menuTapButton->SetSize(_sizeSub);
		_menuTapButton->SetColor(FVector4D::GetColorFromString(COLOR_Transparent));
		_menuTapButton->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		_menuTapButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("OnClickSubCategoryMenuTapButton(): " + std::to_string(i));
				OnClickSubCategoryMenuTapButton(i);
			});
		_menuTap.menuTapButton = _menuTapButton;

		mArrSubCategoryMunuTap[(int)ERankMenuTap::Map].push_back(_menuTap);
	}

	// 서브카테고리, 캐릭터
	int _characterCount = CDataStorageManager::GetInst()->GetCharacterCount();

	for (int i = 0; i < _characterCount; i++)
	{
		FMenuTap _menuTap;
		auto _characterInfo = CDataStorageManager::GetInst()->GetCharacterState(i);

		CSharedPtr<CTextBlock> _menuTapText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_menuTapText");
		AddWidget(_menuTapText);
		_menuTapText->SetPivot(_pivotSub);
		_menuTapText->SetSize(_sizeSub);
		_menuTapText->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapText->SetText(ToUpperWString(_characterInfo.Name).c_str());
		_menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		_menuTapText->SetAlignH(ETextAlignH::Center);
		_menuTapText->SetFontSize(mMenuTextFontSize - 5.0f);
		_menuTapText->SetZOrder(ZORDER_LOBBY_MY_ITEM_ICON);
		_menuTapText->SetEnable(false);
		_menuTap.menuTapText = _menuTapText;

		CSharedPtr<CButton> _menuTapButton = mScene->GetUIManager()->CreateWidget<CButton>("_menuTapButton");
		AddWidget(_menuTapButton);
		_menuTapButton->SetPivot(_pivotSub);
		_menuTapButton->SetSize(_sizeSub);
		_menuTapButton->SetColor(FVector4D::GetColorFromString(COLOR_Transparent));
		_menuTapButton->SetPos(_basePosSub + FVector2D((_sizeSub.x + _gapSubX) * i, 0.0f));
		_menuTapButton->SetZOrder(ZORDER_LOBBY_MY_ITEM_BUTTON);
		_menuTapButton->SetEventCallback(EButtonEventState::Click
			, [this, i]()
			{
				CLog::PrintLog("OnClickSubCategoryMenuTapButton(): " + std::to_string(i));
				OnClickSubCategoryMenuTapButton(i);
			});
		_menuTapButton->SetEnable(false);
		_menuTap.menuTapButton = _menuTapButton;

		mArrSubCategoryMunuTap[(int)ERankMenuTap::Character].push_back(_menuTap);
	}
}

void CResultWidget::InitProperty(FVector2D _basePos)
{
	// 여기서 Row 모양 세팅
	FVector2D _pivot = FVector2D(0.0f, 1.0f);
	FVector2D _sizeName = FVector2D(250.0f, 50.0f);
	FVector2D _sizeMap = FVector2D(200.0f, 50.0f);
	FVector2D _sizeCharacter = FVector2D(250.0f, 50.0f);
	FVector2D _sizeDistance = FVector2D(200.0f, 50.0f);
	FVector2D _sizeItem = FVector2D(200.0f, 50.0f);

	float _gapX = 5.0f;

	FVector2D _posName = _basePos;
	FVector2D _posMap = _posName + FVector2D(_sizeName.x + _gapX, 0.0f);
	FVector2D _posCharacter = _posMap + FVector2D(_sizeMap.x + _gapX, 0.0f);
	FVector2D _posDistance = _posCharacter + FVector2D(_sizeCharacter.x + _gapX, 0.0f);
	FVector2D _posItem = _posDistance + FVector2D(_sizeDistance.x + _gapX + 20.0f, 0.0f);

	FVector4D _color = FVector4D::GetColorFromString(COLOR_White);

	CSharedPtr<CTextBlock> _nameProp = mScene->GetUIManager()->CreateWidget<CTextBlock>("_nameProp");
	AddWidget(_nameProp);
	SetInfoTextBlock(_nameProp, _pivot, _sizeName, _posName, _color, L"NAME");

	CSharedPtr<CTextBlock> _mapProp = mScene->GetUIManager()->CreateWidget<CTextBlock>("_mapProp");
	AddWidget(_mapProp);
	SetInfoTextBlock(_mapProp, _pivot, _sizeMap, _posMap, _color, L"MAP");

	CSharedPtr<CTextBlock> _characterProp = mScene->GetUIManager()->CreateWidget<CTextBlock>("_characterProp");
	AddWidget(_characterProp);
	SetInfoTextBlock(_characterProp, _pivot, _sizeCharacter, _posCharacter, _color, L"CHARACTER");

	CSharedPtr<CTextBlock> _distanceProp = mScene->GetUIManager()->CreateWidget<CTextBlock>("_distanceProp");
	AddWidget(_distanceProp);
	SetInfoTextBlock(_distanceProp, _pivot, _sizeDistance, _posDistance, _color, L"DISTANCE");

	CSharedPtr<CTextBlock> _itemProp = mScene->GetUIManager()->CreateWidget<CTextBlock>("_itemProp");
	AddWidget(_itemProp);
	SetInfoTextBlock(_itemProp, _pivot, _sizeItem, _posItem, _color, L"ITEM");
}

void CResultWidget::InitUserRankPrint(FVector2D _basePos, int _count)
{
	// mRankMaxCount 인원수에 맞게 미리 위치 세팅 -> 테이블 형 포맷만 만듦.
	// 멀티: 다른사람들 스코어.
	// 싱글: 랭킹.
	float _gapY = mRowTextFontSize;

	for (int i = 0; i < _count; i++)
	{
		FUserPrintGroup _group;
		SetPositionUserPrintRow(_group, _basePos + FVector2D(0.0f, -(30.0f + _gapY) * i));
		mArrUserInfoText.push_back(_group);
	}
}

void CResultWidget::InitMyResultScore()
{
	FUserRankInfo _myResultInfo = CDataStorageManager::GetInst()->GetCurUserResult();
	FUserPrintGroup _printGroup;
	SetPositionUserPrintRow(_printGroup, FVector2D(120.0f, 590.0f));

	auto _mapStat = CDataStorageManager::GetInst()->GetMapInfo(_myResultInfo.Map);
	auto _charStat = CDataStorageManager::GetInst()->GetCharacterState(_myResultInfo.Character);

	_printGroup.mapText->SetTextColor(FVector4D::GetColorFromString(_mapStat.ColorName));
	_printGroup.characterText->SetTextColor(FVector4D::GetColorFromString(_charStat.ColorName));

	_printGroup.nameText->SetText(std::wstring(_myResultInfo.Name.begin(), _myResultInfo.Name.end()).c_str());
	_printGroup.mapText->SetText(std::wstring(_mapStat.Name.begin(), _mapStat.Name.end()).c_str());
	_printGroup.characterText->SetText(std::wstring(_charStat.Name.begin(), _charStat.Name.end()).c_str());
	_printGroup.distanceText->SetText((FormatWithCommaManual(_myResultInfo.Distance) + L"m").c_str());

	bool _isEnable = _myResultInfo.Item_0 > PLAYER_ITEM_TYPE_DEFAULT_INDEX;
	_printGroup.arrItemImage[0]->SetEnable(_isEnable);
	if (_isEnable)
		_printGroup.arrItemImage[0]->SetTexture(FItemInfo::GetItmeImageName(_myResultInfo.Item_0), FItemInfo::GetItmeImagePath(_myResultInfo.Item_0));

	_isEnable = _myResultInfo.Item_1 > PLAYER_ITEM_TYPE_DEFAULT_INDEX;
	_printGroup.arrItemImage[1]->SetEnable(_isEnable);
	if (_isEnable)
		_printGroup.arrItemImage[1]->SetTexture(FItemInfo::GetItmeImageName(_myResultInfo.Item_1), FItemInfo::GetItmeImagePath(_myResultInfo.Item_1));

	_isEnable = _myResultInfo.Item_2 > PLAYER_ITEM_TYPE_DEFAULT_INDEX;
	_printGroup.arrItemImage[2]->SetEnable(_isEnable);
	if (_isEnable)
		_printGroup.arrItemImage[2]->SetTexture(FItemInfo::GetItmeImageName(_myResultInfo.Item_2), FItemInfo::GetItmeImagePath(_myResultInfo.Item_2));
}

void CResultWidget::SetPositionUserPrintRow(FUserPrintGroup& _groupOut, FVector2D _basePos)
{
	// 여기서 Row 모양 세팅
	FVector2D _pivot = FVector2D(0.0f, 1.0f);
	FVector2D _sizeName = FVector2D(250.0f, 50.0f);
	FVector2D _sizeMap = FVector2D(200.0f, 50.0f);
	FVector2D _sizeCharacter = FVector2D(250.0f, 50.0f);
	FVector2D _sizeDistance = FVector2D(200.0f, 50.0f);

	float _gapX = 5.0f;

	FVector2D _posName = _basePos;
	FVector2D _posMap = _posName + FVector2D(_sizeName.x + _gapX, 0.0f);
	FVector2D _posCharacter = _posMap + FVector2D(_sizeMap.x + _gapX, 0.0f);
	FVector2D _posDistance = _posCharacter + FVector2D(_sizeCharacter.x + _gapX, 0.0f);

	FVector4D _color = FVector4D::GetColorFromString(COLOR_White);

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
	int _itemCount = CDataStorageManager::GetInst()->GetSelectableItemCount();

	for (int i = 0; i < _itemCount; i++)
	{
		FVector2D _posItem = _posItemBase + FVector2D((_sizeItem.x + _itemGapX) * i, 0.0f);
		CSharedPtr<CImage> _itemSlotImage = mScene->GetUIManager()->CreateWidget<CImage>("_itemSlotImage");
		AddWidget(_itemSlotImage);
		SetImage(_itemSlotImage, _pivotItem, _sizeItem, _posItem, _color, ITEM_EMPTY_SQUARE_NAME, ITEM_EMPTY_SQUARE_PATH);
		_groupOut.arrSlotImage.push_back(_itemSlotImage);

		CSharedPtr<CImage> _itemImage = mScene->GetUIManager()->CreateWidget<CImage>("_itemImage");
		AddWidget(_itemImage);
		SetImage(_itemImage, _pivotItem, _sizeItem * _innerItemSizeRate, _posItem, _color, ITEM_HP_ICON_NAME, ITEM_HP_ICON_PATH);
		_groupOut.arrItemImage.push_back(_itemImage);
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
	_textBlock->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_SlateGray));
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

void CResultWidget::OnClickMainCategoryMenuTapButton(ERankMenuTap::Type _tap)
{
	int _beforeMainCategory = mCurMainCategoryMenuTap;

	// 현재 메인 카테고리 업데이트.
	mCurMainCategoryMenuTap = _tap;

	// 메인카테고리 탭은 색깔 변화
	for (int i = 0; i < mArrMainCategoryMenuTap.size(); i++)
	{
		if (i == (int)mCurMainCategoryMenuTap)
			mArrMainCategoryMenuTap[i].menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		else
			mArrMainCategoryMenuTap[i].menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green) * 0.5f);
	}

	{// 서브카테고리 이전 SetEnable
		auto& _arrSub = mArrSubCategoryMunuTap[_beforeMainCategory];
		for (auto& _sub : _arrSub)
		{
			_sub.menuTapText->SetEnable(false);
			_sub.menuTapButton->SetEnable(false);
		}
	}

	{// 서브카테고리 현재 SetEnable
		auto& _arrSub = mArrSubCategoryMunuTap[mCurMainCategoryMenuTap];
		for (auto& _sub : _arrSub)
		{
			_sub.menuTapText->SetEnable(true);
			_sub.menuTapButton->SetEnable(true);
		}
	}

	{
		// 서브카테고리 선택된 탭 외에 비활성화
		auto& _arrSub = mArrSubCategoryMunuTap[mCurMainCategoryMenuTap];

		for (auto& _sub : _arrSub)
			_sub.menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green) * 0.5f);

		// 서브 카테고리 중 선택된 탭 활성화 -> 색깔변화
		_arrSub[mCurSubCategoryMenuTap[mCurMainCategoryMenuTap]].menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
	}

	// 현재 메인 / 서브 카테고리에 맞는 데이터 가져옴.
	std::vector<FUserRankInfo> _arrRankInfo;
	_arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(
		mCurMainCategoryMenuTap
		, mCurSubCategoryMenuTap[(int)mCurMainCategoryMenuTap]);

	UpdateUserRankPrint(_arrRankInfo);
}

void CResultWidget::OnClickSubCategoryMenuTapButton(int _index)
{
	// 서브 카테고리 업데이트 -> 메인은 냅둔다.
	// 현재 메인 / 서브 카테고리에 맞는 데이터 가져옴.
	const int _menuTapIndex = (int)mCurMainCategoryMenuTap;
	mCurSubCategoryMenuTap[_menuTapIndex] = _index;

	// 서브 메뉴 탭 변경.
	// 현재 메인 카테고리 -> 서브 카테고리 업데이트 -> 서브 카테고리 색 변화.
	auto& _arrSub = mArrSubCategoryMunuTap[_menuTapIndex];
	for (auto& _sub : _arrSub)
		_sub.menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green) * 0.5f);

	_arrSub[mCurSubCategoryMenuTap[_menuTapIndex]].menuTapText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));

	// 데이터 가져오기.
	std::vector<FUserRankInfo> _arrRankInfo;

	_arrRankInfo = CDataStorageManager::GetInst()->GetArrayUserRankByCategory(
		mCurMainCategoryMenuTap
		, mCurSubCategoryMenuTap[_menuTapIndex]
	);

	// Print Row.
	UpdateUserRankPrint(_arrRankInfo);
}

void CResultWidget::UpdateUserRankPrint(std::vector<FUserRankInfo> _arrInfo)
{
	// 세팅해놓은 텍스트나 이미지의 데이터 업데이트.
	// 맥스보다 최소값이 들어올 수 도 있다.
	int _minCount = mRankMaxCount > _arrInfo.size() ? _arrInfo.size() : mRankMaxCount;

	// row 들 모두 꺼야 함.
	for (int i = 0; i < mArrUserInfoText.size(); i++)
	{
		auto& _printGroup = mArrUserInfoText[i];
		_printGroup.nameText->SetEnable(false);
		_printGroup.mapText->SetEnable(false);
		_printGroup.characterText->SetEnable(false);
		_printGroup.distanceText->SetEnable(false);

		_printGroup.arrItemImage[0]->SetEnable(false);
		_printGroup.arrSlotImage[0]->SetEnable(false);
		_printGroup.arrItemImage[1]->SetEnable(false);
		_printGroup.arrSlotImage[1]->SetEnable(false);
		_printGroup.arrItemImage[2]->SetEnable(false);
		_printGroup.arrSlotImage[2]->SetEnable(false);
	}

	for (int i = 0; i < _minCount; i++)
	{
		auto _info = _arrInfo[i];
		auto& _printGroup = mArrUserInfoText[i];
		auto _mapStat = CDataStorageManager::GetInst()->GetMapInfo(_info.Map);
		auto _charStat = CDataStorageManager::GetInst()->GetCharacterState(_info.Character);

		_printGroup.nameText->SetEnable(true);
		_printGroup.mapText->SetEnable(true);
		_printGroup.characterText->SetEnable(true);
		_printGroup.distanceText->SetEnable(true);

		_printGroup.mapText->SetTextColor(FVector4D::GetColorFromString(_mapStat.ColorName));
		_printGroup.characterText->SetTextColor(FVector4D::GetColorFromString(_charStat.ColorName));
		_printGroup.nameText->SetTextColor(FVector4D::GetColorFromString(COLOR_White));

		if (CNetworkManager::GetInst()->IsMultiplay())
		{
			int _myId = CMultiplayManager::GetInst()->GetMyId();
			int _multiplayId = _info.MultiPlayId;

			if(_myId == _multiplayId)
				_printGroup.nameText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		}
		else
		{
			auto _myInfo = CDataStorageManager::GetInst()->GetCurUserResult();

			if (_info.PageId.compare(_myInfo.PageId.c_str()) == 0) 
				_printGroup.nameText->SetTextColor(FVector4D::GetColorFromString(COLOR_Green));
		}

		std::wstring _outName;
		ConvertWStringAndCheckTextOverLength(_info.Name, _outName, PLAYER_RANK_NICKNAME_LENGTH_MAX_SCORE);
		_printGroup.nameText->SetText(_outName.c_str());
		_printGroup.mapText->SetText(std::wstring(_mapStat.Name.begin(), _mapStat.Name.end()).c_str());
		_printGroup.characterText->SetText(std::wstring(_charStat.Name.begin(), _charStat.Name.end()).c_str());
		_printGroup.distanceText->SetText((FormatWithCommaManual(_info.Distance) + L"m").c_str());

		bool _isEnable = _info.Item_0 > PLAYER_ITEM_TYPE_DEFAULT_INDEX;
		_printGroup.arrItemImage[0]->SetEnable(_isEnable);
		_printGroup.arrSlotImage[0]->SetEnable(true);
		if (_isEnable)
			_printGroup.arrItemImage[0]->SetTexture(FItemInfo::GetItmeImageName(_info.Item_0), FItemInfo::GetItmeImagePath(_info.Item_0));

		_isEnable = _info.Item_1 > PLAYER_ITEM_TYPE_DEFAULT_INDEX;
		_printGroup.arrItemImage[1]->SetEnable(_isEnable);
		_printGroup.arrSlotImage[1]->SetEnable(true);
		if (_isEnable)
			_printGroup.arrItemImage[1]->SetTexture(FItemInfo::GetItmeImageName(_info.Item_1), FItemInfo::GetItmeImagePath(_info.Item_1));

		_isEnable = _info.Item_2 > PLAYER_ITEM_TYPE_DEFAULT_INDEX;
		_printGroup.arrItemImage[2]->SetEnable(_isEnable);
		_printGroup.arrSlotImage[2]->SetEnable(true);
		if (_isEnable)
			_printGroup.arrItemImage[2]->SetTexture(FItemInfo::GetItmeImageName(_info.Item_2), FItemInfo::GetItmeImagePath(_info.Item_2));

	}
}

void CResultWidget::SetButton(CButton& _button, const char* _name, const wchar_t* _path)
{
	FVector3D TintNormal = FVector3D(0, 1, 0);
	FVector3D TintHovered = FVector3D(0, 0.8f, 0);
	FVector3D TintClick = FVector3D(0, 0.6f, 0);

	_button.SetTexture(EButtonState::Normal, _name, _path);
	_button.SetTexture(EButtonState::Hovered, _name, _path);
	_button.SetTexture(EButtonState::Click, _name, _path);
	_button.SetPivot(FVector2D::One * 0.5f);
	_button.SetTint(EButtonState::Normal, TintNormal);
	_button.SetTint(EButtonState::Hovered, TintHovered);
	_button.SetTint(EButtonState::Click, TintClick);

}
