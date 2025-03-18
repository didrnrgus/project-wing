#pragma once
#include "Object/SceneObject.h"
#include "Interface/IGamePlayShakeController.h"

class CCameraObject : public CSceneObject, public IGamePlayShakeController
{
	friend class CScene;

protected:
	CCameraObject();
	CCameraObject(const CCameraObject& Obj);
	CCameraObject(CCameraObject&& Obj);
	virtual ~CCameraObject();

protected:
	CSharedPtr<class CCameraComponent> mCamera;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void SetShakeSceneObject(float duration, float intensity) override;
};

