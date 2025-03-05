#include "MousePointer.h"
#include "../Scene/Scene.h"
#include "../Scene/Input.h"
#include "../Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "../Animation/Animation2D.h"

CMousePointer::CMousePointer()
{
}

CMousePointer::CMousePointer(const CMousePointer& Obj)
{
}

CMousePointer::CMousePointer(CMousePointer&& Obj)
{
}

CMousePointer::~CMousePointer()
{
}

bool CMousePointer::Init()
{
    mRoot = CreateComponent<CSpriteComponent>();
    mRoot->SetTexture("Teemo", TEXT("Texture/teemo.png"));
    mRoot->SetPivot(0.0f, 1.0f);
    //mRoot->SetTint(0, 1, 0);

 /*   mBody = CreateComponent<CColliderSphere2D>();
    mBody->SetRadius(100.0f);
    mBody->SetCollisionProfile("Player");

    mRoot->AddChild(mBody);*/

    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();
    mAnimation->AddSequence("MousePointer", 1.f, 6.f, true, false);

    mRoot->SetWorldPos(0.f, 0.f, 0.f);
    FVector3D tempScale = FVector3D(32.0f, 32.0f, 1.f);
    mRoot->SetWorldScale(tempScale);
    SetRootComponent(mRoot);

    
    return true;
}

void CMousePointer::Update(float DeltaTime)
{
    //mAnimation->Update(DeltaTime);
}
