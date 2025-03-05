#pragma once

#include "ColliderBase.h"

class CColliderSphere2D :
    public CColliderBase
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderSphere2D();
	CColliderSphere2D(const CColliderSphere2D& Com);
	CColliderSphere2D(CColliderSphere2D&& Com);
	virtual ~CColliderSphere2D();

protected:
	float	mRadius = 10.f;

public:
	float GetRadius()	const
	{
		return mRadius;
	}

public:
	void SetRadius(float Radius)
	{
		mRadius = Radius;
	}

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderSphere2D* Clone();
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest);
};

