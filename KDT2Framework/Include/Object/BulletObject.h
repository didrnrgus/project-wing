#pragma once

#include "Object/SceneObject.h"

class CBulletObject :
    public CSceneObject
{
    friend class CScene;

protected:
    CBulletObject();
    CBulletObject(const CBulletObject& Obj);
    CBulletObject(CBulletObject&& Obj);
    virtual ~CBulletObject();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CColliderAABB2D>       mBody;
    CSharedPtr<class CMovementComponent>    mMovement;
    float   mSpeed = 400.f;

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

