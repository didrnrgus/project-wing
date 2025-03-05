#pragma once

#include "SceneObject.h"

class CMousePointer :
    public CSceneObject
{
    friend class CScene;

protected:
    CMousePointer();
    CMousePointer(const CMousePointer& Obj);
    CMousePointer(CMousePointer&& Obj);
    virtual ~CMousePointer();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    CSharedPtr<class CColliderSphere2D>  mBody;
    class CAnimation2D* mAnimation = nullptr;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

};

