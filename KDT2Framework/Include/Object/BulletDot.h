#pragma once

#include "Object/SceneObject.h"

class CBulletDot :
    public CSceneObject
{
    friend class CScene;

protected:
    CBulletDot();
    CBulletDot(const CBulletDot& Obj);
    CBulletDot(CBulletDot&& Obj);
    virtual ~CBulletDot();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CColliderOBB2D>       mBody;
    CSharedPtr<class CMovementComponent>    mMovement;
    float       mMoveDist = 200.f;
    float       mDamageTime = 0.f;
    std::list<CSceneObject*>    mCollisionList;

public:
    void SetBulletCollisionProfile(const std::string& Name);
    void SetBoxSize(float x, float y);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void CollisionBullet(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    void CollisionBulletEnd(class CColliderBase* Dest);
};

