#include "PlayerInGameObject.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Etc/ConstString.h"
#include "Interface/IPlayerStatController.h"

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

	if (!mIsMovingUp && mIsMine)
	{
		//CLog::PrintLog("CPlayerInGameObject::Update mIsMovingUp: " + std::to_string(mIsMovingUp));
		MoveDown(DeltaTime);
	}
}

void CPlayerInGameObject::MoveDown(float DeltaTime)
{
	auto downValVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * -1.0f;
	auto pos = mRoot->GetWorldPosition();
	mRoot->SetWorldPos(pos + downValVector);
}

void CPlayerInGameObject::MoveUpStart(float DeltaTime)
{
	mIsMovingUp = true;
	CLog::PrintLog("CPlayerInGameObject::MoveUpStart mIsMovingUp: " + std::to_string(mIsMovingUp));
}

void CPlayerInGameObject::MoveUpHold(float DeltaTime)
{
	//CLog::PrintLog("CPlayerInGameObject::MoveUpHold mIsMovingUp: " + std::to_string(mIsMovingUp));
	auto downValVector = FVector3D::Axis[EAxis::Y] * GetDex() * DeltaTime * 1.0f;
	auto pos = mRoot->GetWorldPosition();
	mRoot->SetWorldPos(pos + downValVector);
}

void CPlayerInGameObject::MoveUpRelease(float DeltaTime)
{
	mIsMovingUp = false;
	CLog::PrintLog("CPlayerInGameObject::MoveUpRelease mIsMovingUp: " + std::to_string(mIsMovingUp));
}

bool CPlayerInGameObject::SetMovePlayer(FVector3D moveValVector)
{
	// 외부 -> 서버데이터로 포지션 컨트롤 
	return false;
}
