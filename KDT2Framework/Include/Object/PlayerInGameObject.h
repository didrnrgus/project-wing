#pragma once
#include "PlayerGraphicObject.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"

class CPlayerInGameObject : public CPlayerGraphicObject, public IPlayerStatController, public IGamePlayStateController
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
	bool mIsMine = false;
	class IGamePlayShakeController* mCameraShake;

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
	

public:
	bool SetMovePlayer(FVector3D moveValVector);
	inline void SetIsMine(bool isMine) { mIsMine = isMine; }
	inline void SetShakeCamera(class IGamePlayShakeController* cameraShake)
	{
		mCameraShake = cameraShake;
	}

	inline bool GetIsMine() { return mIsMine; }
};

