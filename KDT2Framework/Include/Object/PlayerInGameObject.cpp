#include "PlayerInGameObject.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"
#include "Scene/SceneInGame.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IGamePlayShakeController.h"
#include "Etc/DataStorageManager.h"
#include "Etc/TaskManager.h"
#include "Render/RenderManager.h"
#include "Interface/ISceneNetworkController.h"
#include "Etc/NetworkManager.h"
#include "Etc/MultiplayManager.h"
#include "Etc/JsonContainer.h"
#include "Etc/NotionDBController.h"
#include "UI/UserWidget/SceneWidget.h"

extern std::string gNickname;

CPlayerInGameObject::CPlayerInGameObject()
{
}

CPlayerInGameObject::CPlayerInGameObject(const CPlayerInGameObject& Obj)
{
}

CPlayerInGameObject::CPlayerInGameObject(CPlayerInGameObject&& Obj)
{
}

CPlayerInGameObject::~CPlayerInGameObject()
{
	RemoveListener();
}

bool CPlayerInGameObject::Init()
{
	CPlayerGraphicObject::Init();
	AddListener();

	auto scale = mRoot->GetWorldScale();
	mBody = CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	mRoot->AddChild(mBody);
	mBody->SetCollisionProfile(PROFILE_PLAYER_MINE);
	mBody->SetRadius(scale.y * 0.2f);
	mBody->SetCollisionBeginFunc<CPlayerInGameObject>(this, &CPlayerInGameObject::CollisionMapBegin);

	mScene->GetInput()->AddBindKey("MoveUp", VK_LBUTTON);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Down, this, &CPlayerInGameObject::MoveUpStart);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Hold, this, &CPlayerInGameObject::MoveUpHold);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Up, this, &CPlayerInGameObject::MoveUpRelease);

	mScene->GetInput()->AddBindKey("BoostMode", VK_SPACE);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("BoostMode",
		EInputType::Down, this, &CPlayerInGameObject::BoostModeStart);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("BoostMode",
		EInputType::Hold, this, &CPlayerInGameObject::BoostModeHold);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("BoostMode",
		EInputType::Up, this, &CPlayerInGameObject::BoostModeRelease);

	if (!CNetworkManager::GetInst()->IsMultiplay())
		SetPlayerFrezeCallback(this, &CPlayerInGameObject::OnFrezeCallback);

#ifdef _DEBUG
	mScene->GetInput()->AddBindKey("DecreaseHP", 'Z');
	mScene->GetInput()->AddBindFunction("DecreaseHP", EInputType::Down
		, [this](float DeltaTime)
		{
			CLog::PrintLog("mScene->GetInput()->AddBindKey(\"DecreaseHP\", \'Z\');");
			float _damage = CDataStorageManager::GetInst()->GetSelectedMapInfo().CollisionDamage;

			if (CNetworkManager::GetInst()->IsMultiplay())
				SendMessageTriggerFloat(ClientMessage::MSG_TAKE_DAMAGE, _damage);
		});
#endif // _DEBUG

	int itemSlotCount = CDataStorageManager::GetInst()->GetSelectableItemCount();
	auto itemDatas = CDataStorageManager::GetInst()->GetItemInfoDatas();

	// 선택한 캐릭의 초기 스텟 등록.
	auto initializedStatData = CDataStorageManager::GetInst()->GetSelectedCharacterState();
	InitStat(initializedStatData);

	// 아이템 착용한것 플레이어에 스텟 적용. 베이스 스탯에 Add 하는 형식임 이전에 Init되어 있어야 함.
	for (int i = 0; i < itemSlotCount; i++)
	{
		int itemIndexInSlot = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(i);
		if (itemIndexInSlot >= 0)
		{
			// 어떤 스탯에 얼마를 적용할것인지.
			AddValueByStatIndex(
				static_cast<EStatInfoText::Type>(itemDatas[itemIndexInSlot].StatType)
				, itemDatas[itemIndexInSlot].AddValue);
		}
	}

	return true;
}

void CPlayerInGameObject::Update(float DeltaTime)
{
	CPlayerGraphicObject::Update(DeltaTime);

	if (!IsGamePlayEnableByState())
		return;

	if (GetIsProtection())
	{
		mRoot->SetOpacity(0.5f);
		ReleaseProtection(DeltaTime);
	}
	else
	{
#ifdef _DEBUG
		mRoot->SetOpacity(0.8f);
#else
		mRoot->SetOpacity(1.0);
#endif // _DEBUG
	}

	if (GetIsStun())
	{
		ReleaseStun(DeltaTime);
	}

	UpdateDistance(DeltaTime);
	UpdateDecreaseHp(DeltaTime);

	if (!mIsMovingUp && mIsMine)
	{
		//CLog::PrintLog("CPlayerInGameObject::Update mIsMovingUp: " + std::to_string(mIsMovingUp));
		MoveDown(DeltaTime);
	}
}

void CPlayerInGameObject::SetGamePlayState(EGamePlayState::Type type)
{
	IGamePlayStateController::SetGamePlayState(type);

	//CLog::PrintLog("CPlayerInGameObject::SetGamePlayState type: " + std::to_string(type));
}

void CPlayerInGameObject::MoveDown(float DeltaTime)
{
	auto moveValueVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * -1.0f;
	SetMovePlayer(moveValueVector);
}

void CPlayerInGameObject::MoveUpStart(float DeltaTime)
{
	if (!IsPlayerControlEnableByState())
	{
		mIsMovingUp = false;
		return;
	}

	mIsMovingUp = true;
	//CLog::PrintLog("CPlayerInGameObject::MoveUpStart mIsMovingUp: " + std::to_string(mIsMovingUp));

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTrigger(ClientMessage::MSG_MOVE_UP);

}

void CPlayerInGameObject::MoveUpHold(float DeltaTime)
{
	if (!IsPlayerControlEnableByState())
	{
		mIsMovingUp = false;
		return;
	}

	//CLog::PrintLog("CPlayerInGameObject::MoveUpHold mIsMovingUp: " + std::to_string(mIsMovingUp));
	auto moveValueVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * 1.0f;
	SetMovePlayer(moveValueVector);
}

void CPlayerInGameObject::MoveUpRelease(float DeltaTime)
{
	mIsMovingUp = false;
	//CLog::PrintLog("CPlayerInGameObject::MoveUpRelease mIsMovingUp: " + std::to_string(mIsMovingUp));

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTrigger(ClientMessage::MSG_MOVE_DOWN);
}

void CPlayerInGameObject::BoostModeStart(float DeltaTime)
{
	CLog::PrintLog("CPlayerInGameObject::BoostModeStart");
	SetIsBoostMode(true);
	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTrigger(ClientMessage::MSG_BOOST_ON);
}

void CPlayerInGameObject::BoostModeHold(float DeltaTime)
{
	//CLog::PrintLog("CPlayerInGameObject::BoostModeHold");
}

void CPlayerInGameObject::BoostModeRelease(float DeltaTime)
{
	CLog::PrintLog("CPlayerInGameObject::BoostModeRelease");
	SetIsBoostMode(false);
	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTrigger(ClientMessage::MSG_BOOST_OFF);
}

void CPlayerInGameObject::CollisionMapBegin(const FVector3D& HitPoint, CColliderBase* Dest)
{
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	if (GetIsProtection())
		return;

	mCameraShake->SetShakeSceneObject(0.5f, 10.0f);
	float _damage = CDataStorageManager::GetInst()->GetSelectedMapInfo().CollisionDamage;

	if (CNetworkManager::GetInst()->IsMultiplay())
		SendMessageTriggerFloat(ClientMessage::MSG_TAKE_DAMAGE, _damage);
	else
	{
		if (GetCurHP() <= 0.0f)
			OnPlayerDead();
		else
		{
			SetStun();
			Damaged(_damage);
		}
	}

	CLog::PrintLog("CPlayerInGameObject::CollisionMapBegin");
}

void CPlayerInGameObject::SetMovePlayer(FVector3D moveValueVector)
{
	// 자기 자신이 호출
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	auto pos = mRoot->GetWorldPosition();
	auto resultPos = pos + moveValueVector;
	resultPos.y = Clamp(resultPos.y, mResolution.y * -0.5f, mResolution.y * 0.5f);
	mRoot->SetWorldPos(resultPos);
}

//void CPlayerInGameObject::SetMovePlayer(float _height)
//{
//	// 자기 자신이 호출
//	if (!IsGamePlayEnableByState())
//		return;
//
//	if (GetIsStun())
//		return;
//
//	auto pos = mRoot->GetWorldPosition();
//	pos.y = _height;
//	mRoot->SetWorldPos(pos);
//}

void CPlayerInGameObject::UpdateDecreaseHp(float DeltaTime)
{
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	if (GetIsProtection())
		return;

	//CLog::PrintLog("CPlayerInGameObject::UpdateDecreaseHp()");

	if (!CNetworkManager::GetInst()->IsMultiplay())
	{
#ifdef _DEBUG
		DamagedPerDistance(DeltaTime * 10.0f);
#else
		DamagedPerDistance(DeltaTime);
#endif // _DEBUG
	}
}

void CPlayerInGameObject::UpdateDistance(float DeltaTime)
{
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	if (!CNetworkManager::GetInst()->IsMultiplay())
	{
		float speed = GetSpeed();
		float boostMultiplyValue = GetBoostValue();
		float speedPerFrame =
			speed * DeltaTime		// 1초에 얼마만큼 
			* 0.01f					// 미터법
			* boostMultiplyValue;	// 부스트 속도 곱계산. 
		AddPlayDistance(speedPerFrame);
	}
}

void CPlayerInGameObject::OnFrezeCallback()
{
	// 조작 안되고 떨어지기 시작 함.
	CLog::PrintLog("CPlayerInGameObject::OnFrezeCallback()");
	auto curColor = mRoot->GetColor();
	curColor.x *= 0.7f;
	curColor.y *= 0.7f;
	curColor.z *= 0.7f;
	mRoot->SetColor(curColor);

	auto sceneInGame = dynamic_cast<CSceneInGame*>(mScene);
	if (sceneInGame)
	{
		sceneInGame->SetGamePlayState(EGamePlayState::Freze);
	}
}

void CPlayerInGameObject::OnPlayerDead()
{
	CLog::PrintLog("CPlayerInGameObject::OnPlayerDead");

	// 죽은표시 하기 -> 몇초뒤에 Result씬으로 이동
	mDeadSign->SetRelativePos(FVector3D(0.0f, 0.0f, -0.1f));
	mDeadSign->SetEnable(true);

	// 씬한테 그만하라고 전달 -> 씬은 현재 타깃플레이어와 맵에 대해서만 스탑을 해줘야 함.
	auto sceneInGame = dynamic_cast<CSceneInGame*>(mScene);
	if (sceneInGame)
		sceneInGame->SetGamePlayState(EGamePlayState::Dead);

	FUserRankInfo rankInfo;
	rankInfo.Name = gNickname;
	rankInfo.Map = CDataStorageManager::GetInst()->GetSelectedMapIndex();
	rankInfo.Character = CDataStorageManager::GetInst()->GetSelectedCharacterIndex();
	rankInfo.Distance = GetPlayDistance();
	rankInfo.Item_0 = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(0);
	rankInfo.Item_1 = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(1);
	rankInfo.Item_2 = CDataStorageManager::GetInst()->GetCurSelectedItemIDBySlotIndex(2);

	if (!CNetworkManager::GetInst()->IsMultiplay())
	{
		// 쓰레드 시작.
		mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
			[this, rankInfo]()
			{
				CLog::PrintLog("std::this_thread::sleep_for(std::chrono::milliseconds(3000));");
				std::this_thread::sleep_for(std::chrono::milliseconds(3000));

				// 결과 / 랭킹등록 처리
				std::string _newRankPageId = "";
				bool _isNewRecord = CNotionDBController::GetInst()->CreateUserRecord(rankInfo, _newRankPageId);

				FUserRankInfo _rankInfoModified = rankInfo;
				_rankInfoModified.PageId = _newRankPageId;

				CDataStorageManager::GetInst()->SetCurUserResult(_rankInfoModified, _isNewRecord);

				// 결과 씬 이동
				CSceneInGame* _inGameScene = dynamic_cast<CSceneInGame*>(mScene);
				CSceneWidget* _sceneWidget = (CSceneWidget*)_inGameScene->GetInGameWidget();
				_sceneWidget->LoadScene(EGameScene::Result);
				
			})));
	}
}

void CPlayerInGameObject::AddListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->AddListener(this);
}

void CPlayerInGameObject::RemoveListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->RemoveListener(this);
}

void CPlayerInGameObject::ProcessMessage(const RecvMessage& msg)
{
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_PLAYER_DISTANCE:
	{
		if (msg.senderId != GetNetID())
			return;

		auto info = CMultiplayManager::GetInst()->GetPlayerInfoFromMyId();
		SetPlayDistance(info.distance);
		SetDebugText(std::wstring(L"dist: " + std::to_wstring(info.distance) + L"\nhp: " + std::to_wstring(info.curHp)).c_str());
		break;
	}
	case (int)ServerMessage::MSG_TAKEN_DAMAGE:
	{	// 현재 체력 줌.
		if (msg.senderId != GetNetID())
			return;

		auto info = CMultiplayManager::GetInst()->GetPlayerInfoFromMyId();
		SetCurHp(info.curHp);
		SetDebugText(std::wstring(L"dist: " + std::to_wstring(info.distance) + L"\nhp: " + std::to_wstring(info.curHp)).c_str());
		break;
	}
	case (int)ServerMessage::MSG_TAKEN_STUN:
	{
		if (msg.senderId != GetNetID())
			return;

		SetStun();
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_DEAD:
	{
		if (msg.senderId != GetNetID())
			return;

		OnPlayerDead();
		break;
	}
	default:
		break;
	}
}
