#include "GravityBullet.h"
#include "Component/StaticMeshComponent.h"
#include "Component/MovementComponent.h"
#include "MonsterObject.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"

CGravityBullet::CGravityBullet()
{
}

CGravityBullet::CGravityBullet(const CGravityBullet& Obj) :
    CSceneObject(Obj)
{
}

CGravityBullet::CGravityBullet(CGravityBullet&& Obj) :
    CSceneObject(Obj)
{
}

CGravityBullet::~CGravityBullet()
{
}

bool CGravityBullet::Init()
{
    mRoot = CreateComponent<CSpriteComponent>();

    mMovement = CreateComponent<CMovementComponent>();

    mRoot->SetTexture("Bullet", TEXT("Texture/block_ball.png"));
    mRoot->SetPivot(0.5f, 0.5f);
    /*mRoot->SetMesh("CenterRect");
    mRoot->SetShader("ColorMeshShader");*/

    mRoot->SetWorldScale(50.f, 50.f, 1.f);

    SetRootComponent(mRoot);

    mMovement->SetUpdateComponent(mRoot);

    mMovement->SetMoveAxis(EAxis::Y);
    mMovement->SetMoveSpeed(500.f);

    return true;
}

void CGravityBullet::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    mDistance -= mMovement->GetMoveDistance();

    if (mDistance <= 0.f)
    {
        mDuration -= DeltaTime;

        if (mDuration <= 0.f)
            Destroy();

        mMovement->SetEnable(false);

        std::list<CSharedPtr<CMonsterObject>>   MonsterList;

        mScene->FindObjectsFromType<CMonsterObject>(MonsterList);

        auto    iter = MonsterList.begin();
        auto    iterEnd = MonsterList.end();

        for (; iter != iterEnd; ++iter)
        {
            FVector3D Pos = (*iter)->GetWorldPosition();

            float Dist = Pos.Distance(GetWorldPosition());

            if (Dist <= mRange)
            {
                FVector3D   MoveDir;

                switch (mGravityType)
                {
                case EGravityType::Pull:
                    MoveDir = GetWorldPosition() - Pos;
                    break;
                case EGravityType::Push:
                    MoveDir = Pos - GetWorldPosition();
                    break;
                }

                MoveDir.Normalize();

                (*iter)->AddWorldPos(MoveDir * mGravitySpeed * DeltaTime);
            }
        }
    }
}

