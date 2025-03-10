#pragma once
#include "Object/SceneObject.h"

class CPlayerGraphicObject : public CSceneObject
{
	friend class CScene;

protected:
	CPlayerGraphicObject();
	CPlayerGraphicObject(const CPlayerGraphicObject& Obj);
	CPlayerGraphicObject(CPlayerGraphicObject&& Obj);
	virtual ~CPlayerGraphicObject();

protected:
	CSharedPtr<class CSpriteComponent> mRoot;
	CSharedPtr<class CCameraComponent> mCamera;
	class CAnimation2D* mAnimation = nullptr;

	std::vector<FVector4D> mPlayerColors;

	FVector3D mRootInitPos;
	float mMapDifficultySinAngle;
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

public:
	int GetGraphicCount();

	bool SetChangeGraphic(int index);
};
