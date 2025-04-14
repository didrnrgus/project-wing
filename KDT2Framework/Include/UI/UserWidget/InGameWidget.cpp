#include "InGameWidget.h"
#include "Device.h"
#include "Scene/Scene.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneInGame.h"
#include "Scene/Input.h"
#include "UI/Common/Button.h"
#include "UI/Common/Image.h"
#include "UI/Common/TextBlock.h"
#include "Scene/SceneTitle.h"
#include "Etc/DataStorageManager.h"
#include "Etc/MultiplayManager.h"
#include "Etc/NetworkManager.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Object/PlayerInGameObject.h"

CInGameWidget::CInGameWidget()
{
}

CInGameWidget::~CInGameWidget()
{
}

bool CInGameWidget::Init()
{
	CSceneWidget::Init();

	FResolution RS = CDevice::GetInst()->GetResolution();
	auto mapInfo = CDataStorageManager::GetInst()->GetSelectedMapInfo();
	auto inGameScene = dynamic_cast<CSceneInGame*>(mScene);
	if (inGameScene)
	{
		// 내가 조작하는 플레이어를 가져와서 거기에 붙은 스탯을 불러오는것.
		auto player = inGameScene->GetMyPlayerObject();
		mPlayerStat = dynamic_cast<IPlayerStatController*>(player);

		if (mPlayerStat == nullptr)
		{
			CLog::PrintLog("CInGameWidget::Init (mPlayerStat == nullptr) Error");
		}
	}

	InitStartCountAndMyDistanceText();
	InitSelectedItemSlot();
	InitTargetPlayerStat();

	if (CNetworkManager::GetInst()->IsMultiplay())
		InitOtherPlayersInfo();

	return true;
}

void CInGameWidget::Update(float DeltaTime)
{
	CSceneWidget::Update(DeltaTime);

	if (mPlayerStat)
		UpdateTargetPlayerStat(DeltaTime);

	if (CNetworkManager::GetInst()->IsMultiplay())
		UpdateOtherPlayersInfo();
}

void CInGameWidget::InitStartCountAndMyDistanceText()
{
	FResolution RS = CDevice::GetInst()->GetResolution();
	auto mapInfo = CDataStorageManager::GetInst()->GetSelectedMapInfo();

	{ // start count setting
		mStartCountTextArr.push_back(L"START!!");
		mStartCountTextArr.push_back(L"1");
		mStartCountTextArr.push_back(L"2");
		mStartCountTextArr.push_back(L"3");
		mStartCountTextArr.push_back(L"READY~");

		FVector2D pivot = FVector2D(0.5f, 0.5f);
		FVector2D size = FVector2D(1000.0f, 200.5f);
		FVector2D pos = FVector2D(RS.Width, RS.Height) * 0.5f;

		mStartCountText = mScene->GetUIManager()->CreateWidget<CTextBlock>("mStartCountText");
		AddWidget(mStartCountText);
		mStartCountText->SetPivot(pivot);
		mStartCountText->SetSize(size);
		mStartCountText->SetPos(pos);
		mStartCountText->SetText(mStartCountTextArr[mStartCountTextArr.size() - 1]);
		mStartCountText->SetTextColor(FVector4D::GetColorFromString(mapInfo.ColorName));
		mStartCountText->SetFontSize(200.0f);
		mStartCountText->SetAlignH(ETextAlignH::Center);
		mStartCountText->SetShadowEnable(true);
		mStartCountText->SetShadowOffset(10.0f, 10.0f);
		mStartCountText->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mStartCountText->SetZOrder(ZORDER_INGAME_START_COUNT_TEXT);
	}

	{ // play distance text setting
		// mPlayDistanceText
		FVector2D pivot = FVector2D(1.0f, 0.0f);
		FVector2D size = FVector2D(500.0f, 100.0f);
		FVector2D pos = FVector2D(RS.Width - 100.0f, RS.Height * 0.5f);

		mPlayDistanceText = mScene->GetUIManager()->CreateWidget<CTextBlock>("mPlayDistanceText");
		AddWidget(mPlayDistanceText);
		mPlayDistanceText->SetPivot(pivot);
		mPlayDistanceText->SetSize(size);
		mPlayDistanceText->SetPos(pos);
		mPlayDistanceText->SetText((std::to_wstring(0) + L"m").c_str());
		mPlayDistanceText->SetTextColor(FVector4D::GetColorFromString(mapInfo.ColorName));
		mPlayDistanceText->SetFontSize(100.0f);
		mPlayDistanceText->SetAlignH(ETextAlignH::Right);
		mPlayDistanceText->SetShadowEnable(true);
		mPlayDistanceText->SetShadowOffset(5.f, 5.f);
		mPlayDistanceText->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		mPlayDistanceText->SetZOrder(ZORDER_INGAME_START_COUNT_TEXT);
		mPlayDistanceText->SetEnable(false);
	}
}

void CInGameWidget::InitTargetPlayerStat()
{
	FResolution RS = CDevice::GetInst()->GetResolution();
	auto myStatInfo = CDataStorageManager::GetInst()->GetSelectedCharacterState();

	CSharedPtr<CImage> hpBackImage = mScene->GetUIManager()->CreateWidget<CImage>("HPBackImage");
	mHpFrontImage = mScene->GetUIManager()->CreateWidget<CImage>("HPImage");
	AddWidget(hpBackImage);
	AddWidget(mHpFrontImage);

	FVector2D pivot = FVector2D(0.0f, 1.0f);
	FVector2D backSize = FVector2D(RS.Width * 0.4f, 30.0f);
	mHpFrontSize = backSize + FVector2D(-10.0f, -10.0f);
	FVector2D backPos = FVector2D(40.0f, RS.Height - 40.0f);
	FVector2D frontPos = backPos + FVector2D(5.0f, -5.0f);

	hpBackImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	hpBackImage->SetPivot(pivot);
	hpBackImage->SetSize(backSize);
	hpBackImage->SetColor(FVector4D::GetColorFromString(COLOR_LightGray));
	hpBackImage->SetOpacity(0.5f);
	hpBackImage->SetPos(backPos);

	mHpFrontImage->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
	mHpFrontImage->SetPivot(pivot);
	mHpFrontImage->SetSize(mHpFrontSize);
	mHpFrontImage->SetColor(FVector4D::GetColorFromString(myStatInfo.ColorName));
	mHpFrontImage->SetPos(frontPos);
}

void CInGameWidget::InitSelectedItemSlot()
{
	// 아이템 정보 세팅.
// 아이템 아이콘 path setting
	auto itemDatas = CDataStorageManager::GetInst()->GetItemInfoDatas();
	int index = 0;
	for (auto& item : itemDatas)
	{
		mItemImagePaths.push_back(FItemInfo::GetItmeImagePath(index));
		mItemImageNames.push_back(FItemInfo::GetItmeImageName(index));
		index++;
	}

	mItemSlotCount = CDataStorageManager::GetInst()->GetSelectableItemCount();

	// slot image path
	mSlotImagePaths.push_back(ITEM_ADD_SQUARE_PATH);
	mSlotImagePaths.push_back(ITEM_EMPTY_SQUARE_PATH);
	mSlotImageNames.push_back(ITEM_ADD_SQUARE_NAME);
	mSlotImageNames.push_back(ITEM_EMPTY_SQUARE_NAME);

	mSlotPosBase = FVector2D(100.0f, 100.0f);
	mSlotPosAdd = FVector2D(105.0f, 0.0f);
	mSlotSize = FVector2D(100.0f, 100.0f);
	mSlotInnerItemSizeRate = 0.8f;

	auto myStatInfo = CDataStorageManager::GetInst()->GetSelectedCharacterState();

	for (int i = 0; i < mItemSlotCount; i++)
	{
		// 슬롯 이미지.
		FVector2D tempPos = mSlotPosBase + mSlotPosAdd * i;
		CSharedPtr<CImage> buttonBackImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemSlotBack");
		AddWidget(buttonBackImage);
		mItemSlotImages.push_back(buttonBackImage);
		buttonBackImage->SetTexture(mSlotImageNames[(int)SlotType::Added]
			, mSlotImagePaths[(int)SlotType::Added]);
		buttonBackImage->SetPivot(FVector2D::One * 0.5f);
		buttonBackImage->SetSize(mSlotSize);
		buttonBackImage->SetColor(FVector4D::GetColorFromString(myStatInfo.ColorName));
		buttonBackImage->SetPos(tempPos);

		// 슬롯 내부 아이템 이미지.
		int selectedItemIndex = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(i);
		CSharedPtr<CImage> buttonImage = mScene->GetUIManager()->CreateWidget<CImage>("ItmeImage_" + std::to_string(i));
		AddWidget(buttonImage);
		mItemImages.push_back(buttonImage);
		if (selectedItemIndex >= 0)
		{
			buttonImage->SetTexture(mItemImageNames[selectedItemIndex], mItemImagePaths[selectedItemIndex]);
			buttonImage->SetEnable(true);
		}
		else
		{
			buttonImage->SetEnable(false);
			continue;
		}
		buttonImage->SetPivot(FVector2D::One * 0.5f);
		buttonImage->SetSize(mSlotSize * mSlotInnerItemSizeRate * mSlotInnerItemSizeRate);
		buttonImage->SetColor(FVector4D::GetColorFromString(myStatInfo.ColorName));
		buttonImage->SetPos(tempPos);

	}
}

void CInGameWidget::InitOtherPlayersInfo()
{
	auto _otherPlayerCount = CMultiplayManager::GetInst()->GetPlayerCount();
	//auto _otherPlayerCount = 4;
	float _fontSize = 20.0f;
	float _gapV = 3.0f;
	float _gapH = 20.0f;
	FVector2D _basePos = FVector2D(40.0f, 640.0f);

	int _myId = CMultiplayManager::GetInst()->GetMyId();

	auto _sceneInGame = dynamic_cast<CSceneInGame*>(mScene);
	if (!_sceneInGame)
	{
		return;
	}

	for (int i = 0, posIndex = 0; i < _otherPlayerCount; i++, posIndex++)
	{
		auto _playerInfo = CMultiplayManager::GetInst()->GetPlayerInfoByIndex(i);

		if (_playerInfo.id == _myId)
		{
			posIndex--;
			continue;
		}
		FPlayerOtherStatInfo _playerOtherStatInfo;
		_playerOtherStatInfo.id = _playerInfo.id;

		IPlayerStatController* _playerStat = nullptr;
		if (!_sceneInGame->GetOtherPlayerStatByNetId(_playerInfo.id, _playerStat))
			return;

		_playerOtherStatInfo.maxHp =_playerStat->GetMaxHP();

		FVector2D _pivotPlayerNameText = FVector2D(0.0f, 1.0);
		FVector2D _sizePlayerNameText = FVector2D(250.0f, _fontSize);
		FVector2D _posPlayerNameText = _basePos + FVector2D((_sizePlayerNameText.x + _gapH) * posIndex, 0.0f);

		FVector2D _pivotDistText = FVector2D(0.0f, 1.0);
		FVector2D _sizeDistText = FVector2D(200.0f, _fontSize);
		FVector2D _posDistText = _posPlayerNameText - FVector2D(0.0f, _sizePlayerNameText.y + _gapV);

		FVector2D _pivotHpBack = FVector2D(0.0f, 1.0);
		FVector2D _sizeHpBack = FVector2D(200.0f, _fontSize * 0.5f);
		FVector2D _posHpBack = _posDistText - FVector2D(0.0f, _sizePlayerNameText.y + _gapV);

		auto _charInfo = CDataStorageManager::GetInst()->GetCharacterState(_playerInfo.characterType);

		wchar_t _playerNameStr[128] = {};
		std::wstring _nicknameStr;
		ConvertWStringAndCheckTextOverLength(_playerInfo.nickname, _nicknameStr, PLAYER_LOBBY_NICKNAME_LENGTH_MAX_SCORE);
		swprintf_s(_playerNameStr, L"[P%d] %s", _playerInfo.id, _nicknameStr.c_str());

		CSharedPtr<CTextBlock> _playerNameText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_playerNameText_" + std::to_string(_playerInfo.id));
		AddWidget(_playerNameText);
		_playerOtherStatInfo.nameText = _playerNameText;
		_playerNameText->SetPivot(_pivotPlayerNameText);
		_playerNameText->SetSize(_sizePlayerNameText);
		_playerNameText->SetPos(_posPlayerNameText);
		_playerNameText->SetText(_playerNameStr);
		_playerNameText->SetTextColor(FVector4D::GetColorFromString(_charInfo.ColorName));
		_playerNameText->SetFontSize(_fontSize);
		_playerNameText->SetAlignH(ETextAlignH::Left);
		_playerNameText->SetShadowEnable(true);
		_playerNameText->SetShadowOffset(3.f, 3.f);
		_playerNameText->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		_playerNameText->SetZOrder(ZORDER_INGAME_PLAY_OTHER_HP_BACK);

		wchar_t _playerDistStr[64] = {};
		swprintf_s(_playerDistStr, L"Dist: %dm", 0);

		CSharedPtr<CTextBlock> _distanceText = mScene->GetUIManager()->CreateWidget<CTextBlock>("_distanceText_" + std::to_string(_playerInfo.id));
		AddWidget(_distanceText);
		_playerOtherStatInfo.distText = _distanceText;
		_distanceText->SetPivot(_pivotDistText);
		_distanceText->SetSize(_sizeDistText);
		_distanceText->SetPos(_posDistText);
		_distanceText->SetText(_playerDistStr);
		_distanceText->SetTextColor(FVector4D::GetColorFromString(_charInfo.ColorName));
		_distanceText->SetFontSize(_fontSize);
		_distanceText->SetAlignH(ETextAlignH::Left);
		_distanceText->SetShadowEnable(true);
		_distanceText->SetShadowOffset(3.f, 3.f);
		_distanceText->SetTextShadowColor(FVector4D::GetColorFromString(COLOR_DarkGray));
		_distanceText->SetZOrder(ZORDER_INGAME_PLAY_OTHER_HP_BACK);
		
		CSharedPtr<CImage> _playerHpBack = mScene->GetUIManager()->CreateWidget<CImage>("_playerHpBack_" + std::to_string(_playerInfo.id));
		AddWidget(_playerHpBack);
		_playerOtherStatInfo.hpBackImage = _playerHpBack;
		_playerHpBack->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
		_playerHpBack->SetPivot(_pivotHpBack);
		_playerHpBack->SetSize(_sizeHpBack);
		_playerHpBack->SetPos(_posHpBack);
		_playerHpBack->SetColor(FVector4D::GetColorFromString(COLOR_LightGray));
		_playerHpBack->SetZOrder(ZORDER_INGAME_PLAY_OTHER_HP_BACK);

		CSharedPtr<CImage> _playerHp = mScene->GetUIManager()->CreateWidget<CImage>("_playerHp_" + std::to_string(_playerInfo.id));
		AddWidget(_playerHp);
		_playerOtherStatInfo.hpImage = _playerHp;
		_playerHp->SetTexture(TEXTURE_BASIC_NAME, TEXTURE_BASIC_PATH);
		_playerHp->SetPivot(_pivotHpBack);
		_playerHp->SetSize(_sizeHpBack);
		_playerHp->SetPos(_posHpBack);
		_playerHp->SetColor(FVector4D::GetColorFromString(_charInfo.ColorName));
		_playerHp->SetZOrder(ZORDER_INGAME_PLAY_OTHER_HP);

		FVector2D _itemSlotSize = FVector2D(_fontSize * 1.5f, _fontSize * 1.5f);
		FVector2D _itemPosBase = _posHpBack + FVector2D(_fontSize * 1.5f * 0.5f, -_fontSize * 1.5f);
		FVector2D _itemPosAdd = FVector2D(_fontSize * 1.5f + 5.0f, 0.0f);
		float _itemSlotInnerItemSizeRate = 0.65f;
		auto myStatInfo = CDataStorageManager::GetInst()->GetCharacterState(_playerInfo.characterType);

		for (int i = 0; i < mItemSlotCount; i++)
		{
			// 슬롯 이미지.
			FVector2D tempPos = _itemPosBase + _itemPosAdd * i;
			CSharedPtr<CImage> _itemSlotImage = mScene->GetUIManager()->CreateWidget<CImage>("ItemSlotBack");
			AddWidget(_itemSlotImage);
			_playerOtherStatInfo.itemSlotImages.push_back(_itemSlotImage);
			_itemSlotImage->SetTexture(mSlotImageNames[(int)SlotType::Added], mSlotImagePaths[(int)SlotType::Added]);
			_itemSlotImage->SetPivot(FVector2D::One * 0.5f);
			_itemSlotImage->SetSize(_itemSlotSize);
			_itemSlotImage->SetColor(FVector4D::GetColorFromString(myStatInfo.ColorName));
			_itemSlotImage->SetPos(tempPos);
			_itemSlotImage->SetZOrder(ZORDER_INGAME_PLAY_OTHER_HP_BACK);

			// 슬롯 내부 아이템 이미지.
			int selectedItemIndex = _playerInfo.arrItemType[i];
			CSharedPtr<CImage> _itemImage = mScene->GetUIManager()->CreateWidget<CImage>("ItmeImage_" + std::to_string(i));
			AddWidget(_itemImage);
			_playerOtherStatInfo.itemImages.push_back(_itemImage);
			if (selectedItemIndex >= 0)
			{
				_itemImage->SetTexture(mItemImageNames[selectedItemIndex], mItemImagePaths[selectedItemIndex]);
				_itemImage->SetEnable(true);
			}
			else
			{
				_itemImage->SetEnable(false);
				continue;
			}
			_itemImage->SetPivot(FVector2D::One * 0.5f);
			_itemImage->SetSize(_itemSlotSize * _itemSlotInnerItemSizeRate);
			_itemImage->SetColor(FVector4D::GetColorFromString(myStatInfo.ColorName));
			_itemImage->SetPos(tempPos);
			_itemImage->SetZOrder(ZORDER_INGAME_PLAY_OTHER_HP);
		}

		mPlayerOtherStatInfos.push_back(_playerOtherStatInfo);
	}
}

void CInGameWidget::SetStartCount(int count)
{
	if (count < 0)
	{
		mStartCountText->SetEnable(false);
		mPlayDistanceText->SetEnable(true);
		return;
	}

	mStartCountText->SetText(mStartCountTextArr[count]);
}

void CInGameWidget::UpdateTargetPlayerStat(float DeltaTime)
{
	// HP Gauge
	float maxHp = mPlayerStat->GetMaxHP();
	float curHp = mPlayerStat->GetCurHP();
	float maxHpSizeX = mHpFrontSize.x;
	float curHpSizeX = maxHpSizeX * (curHp / maxHp);

	//CLog::PrintLog("CInGameWidget::UpdateTargetPlayerStat maxHp: " + std::to_string(maxHp));
	//CLog::PrintLog("CInGameWidget::UpdateTargetPlayerStat curHp: " + std::to_string(curHp));
	mHpFrontImage->SetSize(FVector2D(curHpSizeX, mHpFrontSize.y));

	// play distance.
	auto playDistanceStr = std::to_wstring((int)mPlayerStat->GetPlayDistance()) + L"m";
	mPlayDistanceText->SetText(playDistanceStr.c_str());
}

void CInGameWidget::UpdateOtherPlayersInfo()
{
	if (GetGamePlayState() == EGamePlayState::Ready
		|| GetGamePlayState() == EGamePlayState::ReadyCount)
		return;

	for (const auto& statInfo : mPlayerOtherStatInfos)
	{
		auto _netInfo = CMultiplayManager::GetInst()->GetPlayerInfoValueById(statInfo.id);
		wchar_t _playerDistStr[64] = {};
		swprintf_s(_playerDistStr, L"Dist: %.0fm", _netInfo.distance);
		statInfo.distText->SetText(_playerDistStr);

		float _rate = _netInfo.curHp / statInfo.maxHp;
		auto _backSize = statInfo.hpBackImage->GetSize();
		float _sizeX = _backSize.x * _rate;
		statInfo.hpImage->SetSize(FVector2D(_sizeX, _backSize.y));
	}
}
