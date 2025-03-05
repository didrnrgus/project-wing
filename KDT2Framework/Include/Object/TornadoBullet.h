#pragma once

#include "Object/SceneObject.h"

class CTornadoBullet :
    public CSceneObject
{
    friend class CScene;

protected:
    CTornadoBullet();
    CTornadoBullet(const CTornadoBullet& Obj);
    CTornadoBullet(CTornadoBullet&& Obj);
    virtual ~CTornadoBullet();

protected:
    CSharedPtr<class CSceneComponent>  mRoot;
    CSharedPtr<class CSceneComponent>  mPivot;
    CSharedPtr<class CSpriteComponent>  mMesh;
    float   mSpeed = 400.f;

public:
    void SetSpeed(float Speed)
    {
        mSpeed = Speed;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

