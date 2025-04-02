#pragma once
#include "PlayerGraphicObject.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IObjectNetworkController.h"

class CPlayerInGameObject : public CPlayerGraphicObject, public IPlayerStatController, public IGamePlayStateController, public IObjectNetworkController
{
	friend class CScene;

protected:
	CPlayerInGameObject();
	CPlayerInGameObject(const CPlayerInGameObject& Obj);
	CPlayerInGameObject(CPlayerInGameObject&& Obj);
	virtual ~CPlayerInGameObject();

protected:
	CSharedPtr<class CColliderSphere2D> mBody;
	
private:
	bool mIsMovingUp = false;
	int mTaskID;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

	virtual void SetGamePlayState(EGamePlayState::Type type) override;
private:
	void MoveDown(float DeltaTime);
	void MoveUpStart(float DeltaTime);
	void MoveUpHold(float DeltaTime);
	void MoveUpRelease(float DeltaTime);

	void BoostModeStart(float DeltaTime);
	void BoostModeHold(float DeltaTime);
	void BoostModeRelease(float DeltaTime);

	void CollisionMapBegin(const FVector3D& HitPoint, class CColliderBase* Dest);
	void SetMovePlayer(FVector3D moveValueVector);
	void SetMovePlayer(float _height);
	void UpdateDecreaseHp(float DeltaTime);
	void UpdateDistance(float DeltaTime);
	void OnFrezeCallback();
	void OnPlayerDead();

public:

	// IObjectNetworkController을(를) 통해 상속됨
	void AddListener() override;
	void RemoveListener() override;
	void ProcessMessage(const RecvMessage& msg) override;
};

