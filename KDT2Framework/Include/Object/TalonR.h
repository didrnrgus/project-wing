#pragma once

#include "Object/SceneObject.h"

enum ETalonRState
{
    Expansion,
    Maintain,
    Reduction
};

class CTalonR :
    public CSceneObject
{
    friend class CScene;

protected:
    CTalonR();
    CTalonR(const CTalonR& Obj);
    CTalonR(CTalonR&& Obj);
    virtual ~CTalonR();

protected:
    CSharedPtr<class CSpriteComponent>  mMesh;

    CSharedPtr<class CMovementComponent>    mMovement;
    CSharedPtr<class CRotationComponent>    mRotation;

    float   mSpeed = 400.f;
    ETalonRState    mState = ETalonRState::Expansion;
    CSharedPtr<CSceneObject> mTarget;

    float           mTime = 5.f;
    float           mTimeAcc = 0.f;
    float           mReadyTime = 0.3f;
    float           mRange = 0.f;
    float           mMaxRange = 300.f;
    float           mRangeLength = 75.f;

public:
    void SetTarget(CSceneObject* Target)
    {
        mTarget = Target;
    }

    void SetSpeed(float Speed)
    {
        mSpeed = Speed;
    }

public:
    virtual bool Init();
    virtual void PreUpdate(float DeltaTime);
    virtual void Update(float DeltaTime);
};

