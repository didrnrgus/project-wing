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
	CSharedPtr<class CSpriteComponent> mDeadSign;
private:
	bool mIsMovingUp = false;
	bool mIsMine = false;
	class IGamePlayShakeController* mCameraShake;
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

	void CollisionMapBegin(const FVector3D& HitPoint, class CColliderBase* Dest);
	void SetMovePlayer(FVector3D moveValueVector, float DeltaTime);
	void UpdateDecreaseHp(float DeltaTime);
	void UpdateDistance(float DeltaTime);
	void OnFrezeCallback();
	void OnPlayerDead();

public:
	// WorldPosition
	void SetMovePlayer(FVector3D moveValueVector);
	inline void SetIsMine(bool isMine) { mIsMine = isMine; }
	inline void SetShakeCamera(class IGamePlayShakeController* cameraShake)
	{
		mCameraShake = cameraShake;
	}

	inline bool GetIsMine() { return mIsMine; }


	// IObjectNetworkController을(를) 통해 상속됨
	void AddListener() override;
	void RemoveListener() override;
	void ProcessMessage(const RecvMessage& msg) override;
};

