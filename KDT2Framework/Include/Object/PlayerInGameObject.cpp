#include "PlayerInGameObject.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"
#include "Scene/SceneInGame.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IGamePlayShakeController.h"
#include "Etc/ConstValues.h"
#include "Etc/DataStorageManager.h"
#include "Etc/TaskManager.h"
#include "Render/RenderManager.h"

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
}

bool CPlayerInGameObject::Init()
{
	CPlayerGraphicObject::Init();

	auto scale = mRoot->GetWorldScale();
	mBody = CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	mRoot->AddChild(mBody);
	mBody->SetCollisionProfile(PROFILE_PLAYER_MINE);
	mBody->SetRadius(scale.y * 0.2f);
	mBody->SetCollisionBeginFunc<CPlayerInGameObject>(this, &CPlayerInGameObject::CollisionMapBegin);

	mDeadSign = CreateComponent<CSpriteComponent>(TEXTURE_NAME_DEAD_SIGN);
	mRoot->AddChild(mDeadSign);
	mDeadSign->SetTexture(TEXTURE_NAME_DEAD_SIGN, TEXTURE_PATH_DEAD_SIGN);
	mDeadSign->SetPivot(FVector2D::One * 0.5f);
	mDeadSign->SetWorldScale(FVector2D::One * 128.0f * 0.7f);
	//CRenderManager::GetInst()->MoveRenderElement(mDeadSign, true);
	mDeadSign->SetEnable(false);

	mScene->GetInput()->AddBindKey("MoveUp", VK_LBUTTON);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Down, this, &CPlayerInGameObject::MoveUpStart);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Hold, this, &CPlayerInGameObject::MoveUpHold);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Up, this, &CPlayerInGameObject::MoveUpRelease);

	SetPlayerDeadCallback(this, &CPlayerInGameObject::OnDeadCallback);

#ifdef _DEBUG
	mScene->GetInput()->AddBindKey("DecreaseHP", 'Z');
	mScene->GetInput()->AddBindFunction("DecreaseHP", EInputType::Down
		, [this](float DeltaTime)
		{
			CLog::PrintLog("mScene->GetInput()->AddBindKey(\"DecreaseHP\", 'Z');");
			Damaged(20.0f);
		});
#endif // _DEBUG


	return true;
}

void CPlayerInGameObject::Update(float DeltaTime)
{
	CPlayerGraphicObject::Update(DeltaTime);

	if (GetGamePlayState() != EGamePlayState::Start)
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
	SetMovePlayer(moveValueVector, DeltaTime);
}

void CPlayerInGameObject::MoveUpStart(float DeltaTime)
{
	mIsMovingUp = true;
	//CLog::PrintLog("CPlayerInGameObject::MoveUpStart mIsMovingUp: " + std::to_string(mIsMovingUp));
}

void CPlayerInGameObject::MoveUpHold(float DeltaTime)
{
	//CLog::PrintLog("CPlayerInGameObject::MoveUpHold mIsMovingUp: " + std::to_string(mIsMovingUp));
	auto moveValueVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * 1.0f;
	SetMovePlayer(moveValueVector, DeltaTime);
}

void CPlayerInGameObject::MoveUpRelease(float DeltaTime)
{
	mIsMovingUp = false;
	//CLog::PrintLog("CPlayerInGameObject::MoveUpRelease mIsMovingUp: " + std::to_string(mIsMovingUp));
}

void CPlayerInGameObject::CollisionMapBegin(const FVector3D& HitPoint, CColliderBase* Dest)
{
	if (GetGamePlayState() != EGamePlayState::Start)
		return;

	if (GetIsStun())
		return;

	if (GetIsProtection())
		return;

	mCameraShake->SetShakeSceneObject(0.5f, 10.0f);
	SetStun();
	Damaged(CDataStorageManager::GetInst()->GetSelectedMapInfo().CollisionDamage);
	CLog::PrintLog("CPlayerInGameObject::CollisionMap");
}

void CPlayerInGameObject::SetMovePlayer(FVector3D moveValueVector, float DeltaTime)
{
	// 자기 자신이 호출
	if (GetGamePlayState() != EGamePlayState::Start)
		return;

	if (GetIsStun())
		return;

	auto pos = mRoot->GetWorldPosition();
	mRoot->SetWorldPos(pos + moveValueVector);
}

void CPlayerInGameObject::UpdateDecreaseHp(float DeltaTime)
{
	if (GetGamePlayState() != EGamePlayState::Start)
		return;

	if (GetIsStun())
		return;

	if (GetIsProtection())
		return;

	//CLog::PrintLog("CPlayerInGameObject::UpdateDecreaseHp()");

#ifdef _DEBUG
	DamagedPerDistance(DeltaTime * 10.0f);
#else
	DamagedPerDistance(DeltaTime);
#endif // _DEBUG
}

void CPlayerInGameObject::OnDeadCallback()
{
	CLog::PrintLog("CPlayerInGameObject::OnDeadCallback()");
	// 죽은표시 하기 -> 몇초뒤에 Result씬으로 이동
	mDeadSign->SetRelativePos(FVector3D(0.0f, 0.0f, -0.1f));
	mDeadSign->SetEnable(true);

	// 씬한테 그만하라고 전달 -> 씬은 현재 타깃플레이어와 맵에 대해서만 스탑을 해줘야 함.
	auto sceneInGame = dynamic_cast<CSceneInGame*>(mScene);
	if (sceneInGame)
	{
		sceneInGame->SetGamePlayState(EGamePlayState::Dead);
	}

	// 쓰레드 시작.
	mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
		[this]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));

			CSceneManager::GetInst()->CreateLoadScene<CSceneLobby>();
			CLog::PrintLog("std::this_thread::sleep_for(std::chrono::milliseconds(3000));");
			CLog::PrintLog("NEXT SCENE -> GAME_RESULT");
		})));
}

void CPlayerInGameObject::SetMovePlayer(FVector3D moveValueVector)
{
	// 외부 -> 서버데이터로 포지션 컨트롤 
	// 씬에서 호출
	mRoot->SetWorldPos(moveValueVector);
}
