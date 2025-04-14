#pragma once
#include "PlayerGraphicObject.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"

class CPlayerTitleObject : public CPlayerGraphicObject, public IPlayerStatController, public IGamePlayStateController
{
	friend class CScene;

protected:
	CPlayerTitleObject();
	CPlayerTitleObject(const CPlayerTitleObject& Obj);
	CPlayerTitleObject(CPlayerTitleObject&& Obj);
	virtual ~CPlayerTitleObject();

protected:
	CSharedPtr<class CColliderSphere2D> mBody;
	bool mIsMovingUp = false;

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

};

