#pragma once

#include "Component.h"

class CRotationComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CRotationComponent();
	CRotationComponent(const CRotationComponent& Com);
	CRotationComponent(CRotationComponent&& Com);
	virtual ~CRotationComponent();

protected:
	CSharedPtr<class CSceneComponent>	mUpdateComponent;

protected:
	FVector3D		mVelocity;
	FVector3D		mRotationStep;
	bool			mVelocityInit = true;

public:
	const FVector3D& GetRotationStep()	const
	{
		return mRotationStep;
	}

	float GetRotationDistance()	const
	{
		return mRotationStep.Length();
	}

public:
	void SetUpdateComponent(class CSceneComponent* Target);

	void AddMove(const FVector3D& Rot)
	{
		mVelocity += Rot;
	}

	void AddMoveX(float x)
	{
		mVelocity.x += x;
	}

	void AddMoveY(float y)
	{
		mVelocity.y += y;
	}

	void AddMoveZ(float z)
	{
		mVelocity.z += z;
	}

	void SetMove(const FVector3D& Rot)
	{
		mVelocity = Rot;
	}

	void SetMoveX(float x)
	{
		mVelocity.x = x;
	}

	void SetMoveY(float y)
	{
		mVelocity.y = y;
	}

	void SetMoveZ(float z)
	{
		mVelocity.z = z;
	}

	void SetVelocityInit(bool VelocityInit)
	{
		mVelocityInit = VelocityInit;
	}

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRotationComponent* Clone();
	virtual void EndFrame();
};

