#include "PlayerTitleObject.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IGamePlayShakeController.h"
#include "Etc/DataStorageManager.h"
#include "Render/RenderManager.h"
#include "Etc/JsonContainer.h"
#include "Component/WidgetComponent.h"

CPlayerTitleObject::CPlayerTitleObject()
{
}

CPlayerTitleObject::CPlayerTitleObject(const CPlayerTitleObject& Obj)
{
}

CPlayerTitleObject::CPlayerTitleObject(CPlayerTitleObject&& Obj)
{
}

CPlayerTitleObject::~CPlayerTitleObject()
{
}

bool CPlayerTitleObject::Init()
{
	CPlayerGraphicObject::Init();

	auto scale = mRoot->GetWorldScale();
	mBody = CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	mRoot->AddChild(mBody);
	mBody->SetCollisionProfile(PROFILE_PLAYER_MINE);
	mBody->SetRadius(scale.y * 0.2f);
	mBody->SetCollisionBeginFunc<CPlayerTitleObject>(this, &CPlayerTitleObject::CollisionMapBegin);

	mScene->GetInput()->AddBindKey("MoveUp", VK_LBUTTON);
	mScene->GetInput()->AddBindFunction<CPlayerTitleObject>("MoveUp",
		EInputType::Down, this, &CPlayerTitleObject::MoveUpStart);
	mScene->GetInput()->AddBindFunction<CPlayerTitleObject>("MoveUp",
		EInputType::Hold, this, &CPlayerTitleObject::MoveUpHold);
	mScene->GetInput()->AddBindFunction<CPlayerTitleObject>("MoveUp",
		EInputType::Up, this, &CPlayerTitleObject::MoveUpRelease);

	mScene->GetInput()->AddBindKey("BoostMode", VK_SPACE);
	mScene->GetInput()->AddBindFunction<CPlayerTitleObject>("BoostMode",
		EInputType::Down, this, &CPlayerTitleObject::BoostModeStart);
	mScene->GetInput()->AddBindFunction<CPlayerTitleObject>("BoostMode",
		EInputType::Hold, this, &CPlayerTitleObject::BoostModeHold);
	mScene->GetInput()->AddBindFunction<CPlayerTitleObject>("BoostMode",
		EInputType::Up, this, &CPlayerTitleObject::BoostModeRelease);

	// 선택한 캐릭의 초기 스텟 등록.
	auto initializedStatData = CDataStorageManager::GetInst()->GetCharacterState(0);
	InitStat(initializedStatData);

	mDebugTextComp->SetEnable(false);
	return true;
}

void CPlayerTitleObject::Update(float DeltaTime)
{
	CPlayerGraphicObject::Update(DeltaTime);

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

	if (!mIsMovingUp && mIsMine)
	{
		MoveDown(DeltaTime);
	}
}

void CPlayerTitleObject::SetGamePlayState(EGamePlayState::Type type)
{
	IGamePlayStateController::SetGamePlayState(type);
}

void CPlayerTitleObject::MoveDown(float DeltaTime)
{
	auto moveValueVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * -1.0f;
	SetMovePlayer(moveValueVector);
}

void CPlayerTitleObject::MoveUpStart(float DeltaTime)
{
	if (!IsPlayerControlEnableByState())
	{
		mIsMovingUp = false;
		return;
	}

	mIsMovingUp = true;
	//CLog::PrintLog("CPlayerInGameObject::MoveUpStart mIsMovingUp: " + std::to_string(mIsMovingUp));

}

void CPlayerTitleObject::MoveUpHold(float DeltaTime)
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

void CPlayerTitleObject::MoveUpRelease(float DeltaTime)
{
	mIsMovingUp = false;
	//CLog::PrintLog("CPlayerInGameObject::MoveUpRelease mIsMovingUp: " + std::to_string(mIsMovingUp));
}

void CPlayerTitleObject::BoostModeStart(float DeltaTime)
{
	CLog::PrintLog("CPlayerInGameObject::BoostModeStart");
	SetIsBoostMode(true);
}

void CPlayerTitleObject::BoostModeHold(float DeltaTime)
{
	//CLog::PrintLog("CPlayerInGameObject::BoostModeHold");
}

void CPlayerTitleObject::BoostModeRelease(float DeltaTime)
{
	CLog::PrintLog("CPlayerInGameObject::BoostModeRelease");
	SetIsBoostMode(false);
}

void CPlayerTitleObject::CollisionMapBegin(const FVector3D& HitPoint, CColliderBase* Dest)
{
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	if (GetIsProtection())
		return;

	mCameraShake->SetShakeSceneObject(0.5f, 10.0f);

	SetStun();
	CLog::PrintLog("CPlayerInGameObject::CollisionMapBegin");
}

void CPlayerTitleObject::SetMovePlayer(FVector3D moveValueVector)
{
	// 자기 자신이 호출
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	auto pos = mRoot->GetWorldPosition();
	auto resultPos = pos + moveValueVector;
	resultPos.y = Clamp(resultPos.y, -120.0f, mResolution.y * 0.5f);
	mRoot->SetWorldPos(resultPos);
}

void CPlayerTitleObject::SetMovePlayer(float _height)
{
	// 자기 자신이 호출
	if (!IsGamePlayEnableByState())
		return;

	if (GetIsStun())
		return;

	auto pos = mRoot->GetWorldPosition();
	pos.y = _height;
	mRoot->SetWorldPos(pos);
}

