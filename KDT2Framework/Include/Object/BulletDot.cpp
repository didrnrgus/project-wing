#include "BulletDot.h"
#include "Component/StaticMeshComponent.h"
#include "Component/MovementComponent.h"
#include "Component/ColliderOBB2D.h"
#include "Share/Log.h"
#include "Component/SpriteComponent.h"

CBulletDot::CBulletDot()
{
}

CBulletDot::CBulletDot(const CBulletDot& Obj) :
    CSceneObject(Obj)
{
}

CBulletDot::CBulletDot(CBulletDot&& Obj) :
    CSceneObject(Obj)
{
}

CBulletDot::~CBulletDot()
{
    //mBody->ClearEndFunction();
}

void CBulletDot::SetBulletCollisionProfile(
    const std::string& Name)
{
    mBody->SetCollisionProfile(Name);
}

void CBulletDot::SetBoxSize(float x, float y)
{
    mBody->SetBoxSize(x, y);
}

bool CBulletDot::Init()
{
    mRoot = CreateComponent<CSpriteComponent>();
    mBody = CreateComponent<CColliderOBB2D>();

    mBody->SetEnable(false);

    mMovement = CreateComponent<CMovementComponent>();

    mRoot->SetTexture("Bullet", TEXT("Texture/block_ball.png"));
    mRoot->SetPivot(0.5f, 0.5f);
    /*mRoot->SetMesh("CenterRect");
    mRoot->SetShader("ColorMeshShader");*/

    mRoot->SetWorldScale(0.5f, 0.5f, 1.f);

    SetRootComponent(mRoot);

    //mBody->SetBoxSize(50.f, 50.f);

    mBody->SetCollisionBeginFunc<CBulletDot>(this,
        &CBulletDot::CollisionBullet);
    mBody->SetCollisionEndFunc<CBulletDot>(this,
        &CBulletDot::CollisionBulletEnd);

    mRoot->AddChild(mBody);

    mMovement->SetUpdateComponent(mRoot);

    mMovement->SetMoveAxis(EAxis::Y);
    mMovement->SetMoveSpeed(500.f);

    return true;
}

void CBulletDot::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    // 이동 거리가 남았을 경우
    if (mMoveDist > 0.f)
    {
        mMoveDist -= mMovement->GetMoveDistance();

        // 이동해야할 거리를 모두 이동했을 경우
        if (mMoveDist <= 0.f)
        {
            mMovement->SetEnable(false);
            mBody->SetEnable(true);
            SetLifeTime(5.f);
        }
    }

    // 이동거리를 모두 이동했을 경우
    else
    {
        mDamageTime += DeltaTime;

        if (mDamageTime >= 1.f)
        {
            mDamageTime -= 1.f;

            auto    iter = mCollisionList.begin();
            auto    iterEnd = mCollisionList.end();

            for (; iter != iterEnd; ++iter)
            {
                (*iter)->Damage(1.f, this);
            }
        }
    }

    /*FVector3D Pos = mRoot->GetWorldPosition();

    Pos += mRoot->GetAxis(EAxis::Y) * mSpeed * DeltaTime;

    mRoot->SetWorldPos(Pos);*/
}

void CBulletDot::CollisionBullet(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    mCollisionList.emplace_back(Dest->GetOwner());
}

void CBulletDot::CollisionBulletEnd(CColliderBase* Dest)
{
    auto    iter = mCollisionList.begin();
    auto    iterEnd = mCollisionList.end();

    for (; iter != iterEnd; ++iter)
    {
        if (*iter == Dest->GetOwner())
        {
            mCollisionList.erase(iter);
            break;
        }
    }
}

