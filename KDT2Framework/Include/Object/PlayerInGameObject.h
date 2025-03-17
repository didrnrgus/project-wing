#pragma once
#include "PlayerGraphicObject.h"

class CPlayerInGameObject : public CPlayerGraphicObject
{
	friend class CScene;

protected:
	CPlayerInGameObject();
	CPlayerInGameObject(const CPlayerInGameObject& Obj);
	CPlayerInGameObject(CPlayerInGameObject&& Obj);
	virtual ~CPlayerInGameObject();

private:
	bool mIsMovingUp = false;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

private:
	void MoveDown(float DeltaTime);
	void MoveUpStart(float DeltaTime);
	void MoveUpHold(float DeltaTime);
	void MoveUpRelease(float DeltaTime);
};

