#pragma once

#include "Component.h"

class CMovementComponent :
    public CComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CMovementComponent();
	CMovementComponent(const CMovementComponent& Com);
	CMovementComponent(CMovementComponent&& Com);
	virtual ~CMovementComponent();

protected:
	CSharedPtr<class CSceneComponent>	mUpdateComponent;
	EAxis::Type		mMoveAxis = EAxis::None;

protected:
	FVector3D		mVelocity;
	FVector3D		mMoveStep;
	float			mSpeed = 1.f;
	bool			mVelocityInit = true;

public:
	const FVector3D& GetMoveStep()	const
	{
		return mMoveStep;
	}

	const FVector3D& GetVelocity()	const
	{
		return mVelocity;
	}

	float GetVelocityLength()	const
	{
		return mVelocity.Length();
	}

	float GetMoveDistance()	const
	{
		return mMoveStep.Length();
	}

public:
	void SetUpdateComponent(class CSceneComponent* Target);
	void SetMoveSpeed(float Speed)
	{
		mSpeed = Speed;
	}

	void AddMove(const FVector3D& Dir)
	{
		mVelocity += Dir;
	}

	void SetMove(const FVector3D& Dir)
	{
		mVelocity = Dir;
	}

	void SetVelocityInit(bool VelocityInit)
	{
		mVelocityInit = VelocityInit;
	}

	void SetMoveAxis(EAxis::Type Axis)
	{
		mMoveAxis = Axis;
	}

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CMovementComponent* Clone();
	virtual void EndFrame();
};


