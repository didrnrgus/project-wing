#pragma once

#include "Object/SceneObject.h"

class CPenetrationBullet :
    public CSceneObject
{
    friend class CScene;

protected:
    CPenetrationBullet();
    CPenetrationBullet(const CPenetrationBullet& Obj);
    CPenetrationBullet(CPenetrationBullet&& Obj);
    virtual ~CPenetrationBullet();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CColliderAABB2D>       mBody;
    CSharedPtr<class CMovementComponent>    mMovement;
    float   mSpeed = 400.f;
    float   mMoveDistance = 1000.f;

    std::vector<class CColliderBase*>   mHitList;
    int     mHitCount = 2;

public:
    void SetSpeed(float Speed)
    {
        mSpeed = Speed;
    }

    void SetBulletCollisionProfile(const std::string& Name);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);



private:
    void CollisionBullet(const FVector3D& HitPoint,
        class CColliderBase* Dest);
};

