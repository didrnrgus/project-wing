#pragma once

#include "Object/SceneObject.h"

enum class EGravityType
{
    Pull,
    Push
};

class CGravityBullet :
    public CSceneObject
{
    friend class CScene;

protected:
    CGravityBullet();
    CGravityBullet(const CGravityBullet& Obj);
    CGravityBullet(CGravityBullet&& Obj);
    virtual ~CGravityBullet();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CMovementComponent>    mMovement;
    EGravityType       mGravityType = EGravityType::Pull;
    float   mSpeed = 400.f;
    float   mDistance = 150.f;
    float   mRange = 500.f;
    float   mDuration = 1.f;
    float   mGravitySpeed = 400.f;

public:
    void SetSpeed(float Speed)
    {
        mSpeed = Speed;
    }

    void SetGravityType(EGravityType Type)
    {
        mGravityType = Type;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

