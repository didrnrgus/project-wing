#pragma once
#include "Object/SceneObject.h"

class CCameraObject : public CSceneObject
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

};

