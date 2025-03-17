#pragma once
#include "PlayerGraphicObject.h"
#include "Interface/IPlayerStatController.h"

class CPlayerInGameObject : public CPlayerGraphicObject, public IPlayerStatController
{
	friend class CScene;

protected:
	CPlayerInGameObject();
	CPlayerInGameObject(const CPlayerInGameObject& Obj);
	CPlayerInGameObject(CPlayerInGameObject&& Obj);
	virtual ~CPlayerInGameObject();

protected:
	CSharedPtr<class CColliderOBB2D> mBody;

private:
	bool mIsMovingUp = false;
	bool mIsMine = false;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

private:
	void MoveDown(float DeltaTime);
	void MoveUpStart(float DeltaTime);
	void MoveUpHold(float DeltaTime);
	void MoveUpRelease(float DeltaTime);

public:
	inline void SetIsMine(bool isMine) { mIsMine = isMine; }
	bool SetMovePlayer(FVector3D moveValVector);

	inline bool GetIsMine() { return mIsMine; }
};

