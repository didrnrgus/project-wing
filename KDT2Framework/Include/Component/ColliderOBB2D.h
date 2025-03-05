#pragma once

#include "ColliderBase.h"

class CColliderOBB2D :
    public CColliderBase
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderOBB2D();
	CColliderOBB2D(const CColliderOBB2D& Com);
	CColliderOBB2D(CColliderOBB2D&& Com);
	virtual ~CColliderOBB2D();

protected:
	FVector2D	mBoxSize;
	FOBB2D		mBoxInfo;

public:
	const FOBB2D& GetBox()	const
	{
		return mBoxInfo;
	}

	const FVector2D& GetBoxSize()	const
	{
		return mBoxSize;
	}

public:
	void SetBoxSize(const FVector2D& Size)
	{
		mBoxSize = Size;
	}

	void SetBoxSize(float x, float y)
	{
		mBoxSize.x = x;
		mBoxSize.y = y;
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
	virtual CColliderOBB2D* Clone();
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest);
};

