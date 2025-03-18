#include "PlayerInGameObject.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Etc/ConstString.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IGamePlayShakeController.h"

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

	mBody = CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	mRoot->AddChild(mBody);
	auto scale = mRoot->GetWorldScale();
	mBody->SetCollisionProfile(PROFILE_PLAYER_MINE);
	mBody->SetRadius(scale.y * 0.2f);
	
	mBody->SetCollisionBeginFunc<CPlayerInGameObject>(this, &CPlayerInGameObject::CollisionMapBegin);
	mBody->SetCollisionEndFunc(
		[](CColliderBase* Dest)
		{
			CLog::PrintLog("mBody->SetCollisionEndFunc");
		});

	mScene->GetInput()->AddBindKey("MoveUp", VK_LBUTTON);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Down, this, &CPlayerInGameObject::MoveUpStart);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Hold, this, &CPlayerInGameObject::MoveUpHold);
	mScene->GetInput()->AddBindFunction<CPlayerInGameObject>("MoveUp",
		EInputType::Up, this, &CPlayerInGameObject::MoveUpRelease);
	return true;
}

void CPlayerInGameObject::Update(float DeltaTime)
{
	CPlayerGraphicObject::Update(DeltaTime);

	if (GetGamePlayState() < EGamePlayState::Start)
		return;

	if (GetIsProtection())
	{
		ReleaseProtection(DeltaTime);
	}

	if (GetIsStun())
	{
		ReleaseStun(DeltaTime);
	}

	if (!mIsMovingUp && mIsMine)
	{
		//CLog::PrintLog("CPlayerInGameObject::Update mIsMovingUp: " + std::to_string(mIsMovingUp));
		MoveDown(DeltaTime);
	}
}

void CPlayerInGameObject::SetGamePlayState(EGamePlayState::Type type)
{
	IGamePlayStateController::SetGamePlayState(type);

	CLog::PrintLog("CPlayerInGameObject::SetGamePlayState type: " + std::to_string(type));
}

void CPlayerInGameObject::MoveDown(float DeltaTime)
{
	auto moveValueVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * -1.0f;
	SetMovePlayer(moveValueVector, DeltaTime);
}

void CPlayerInGameObject::MoveUpStart(float DeltaTime)
{
	mIsMovingUp = true;
	CLog::PrintLog("CPlayerInGameObject::MoveUpStart mIsMovingUp: " + std::to_string(mIsMovingUp));
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
	CLog::PrintLog("CPlayerInGameObject::MoveUpRelease mIsMovingUp: " + std::to_string(mIsMovingUp));
}

void CPlayerInGameObject::CollisionMapBegin(const FVector3D& HitPoint, CColliderBase* Dest)
{
	if (GetGamePlayState() < EGamePlayState::Start)
		return;

	if (GetIsStun())
		return;

	if (GetIsProtection())
		return;

	mCameraShake->SetShakeSceneObject(0.5f, 10.0f);
	SetStun();
	CLog::PrintLog("CPlayerInGameObject::CollisionMap");
}

void CPlayerInGameObject::SetMovePlayer(FVector3D moveValueVector, float DeltaTime)
{
	// 자기 자신이 호출
	if (GetGamePlayState() < EGamePlayState::Start)
		return;

	if (GetIsStun())
		return;

	auto pos = mRoot->GetWorldPosition();
	mRoot->SetWorldPos(pos + moveValueVector);
}

void CPlayerInGameObject::SetMovePlayer(FVector3D moveValueVector)
{
	// 외부 -> 서버데이터로 포지션 컨트롤 
	// 씬에서 호출
	mRoot->SetWorldPos(moveValueVector);
}
