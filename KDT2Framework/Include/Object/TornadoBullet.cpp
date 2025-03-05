#include "TornadoBullet.h"
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"

CTornadoBullet::CTornadoBullet()
{
}

CTornadoBullet::CTornadoBullet(const CTornadoBullet& Obj) :
    CSceneObject(Obj)
{
}

CTornadoBullet::CTornadoBullet(CTornadoBullet&& Obj) :
    CSceneObject(Obj)
{
}

CTornadoBullet::~CTornadoBullet()
{
}

bool CTornadoBullet::Init()
{
    mRoot = CreateComponent<CSceneComponent>();
    mPivot = CreateComponent<CSceneComponent>();
    mMesh = CreateComponent<CSpriteComponent>();

    mMesh->SetTexture("Bullet", TEXT("Texture/block_ball.png"));
    mMesh->SetPivot(0.5f, 0.5f);
    /*mMesh->SetMesh("CenterRect");
    mMesh->SetShader("ColorMeshShader");*/

    mMesh->SetWorldScale(0.5f, 0.5f, 1.f);
    mMesh->SetRelativePos(-1.5f, 0.f);

    SetRootComponent(mRoot);

    mRoot->AddChild(mPivot);
    mPivot->AddChild(mMesh);

    return true;
}

void CTornadoBullet::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    FVector3D Pos = mRoot->GetWorldPosition();

    Pos += mRoot->GetAxis(EAxis::Y) * mSpeed * DeltaTime;

    mRoot->SetWorldPos(Pos);

    FVector3D Rot = mPivot->GetRelativeRotation();

    Rot.z += DeltaTime * 720.f;

    mPivot->SetRelativeRotationZ(Rot.z);
}

