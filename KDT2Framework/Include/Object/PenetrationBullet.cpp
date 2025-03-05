#include "PenetrationBullet.h"
#include "Component/StaticMeshComponent.h"
#include "Component/MovementComponent.h"
#include "../Component/ColliderAABB2D.h"
#include "Share/Log.h"
#include "Component/SpriteComponent.h"

CPenetrationBullet::CPenetrationBullet()
{
}

CPenetrationBullet::CPenetrationBullet(const CPenetrationBullet& Obj) :
    CSceneObject(Obj)
{
}

CPenetrationBullet::CPenetrationBullet(CPenetrationBullet&& Obj) :
    CSceneObject(Obj)
{
}

CPenetrationBullet::~CPenetrationBullet()
{
}

void CPenetrationBullet::SetBulletCollisionProfile(
    const std::string& Name)
{
    mBody->SetCollisionProfile(Name);
}

bool CPenetrationBullet::Init()
{
    mRoot = CreateComponent<CSpriteComponent>();
    mBody = CreateComponent<CColliderAABB2D>();

    mMovement = CreateComponent<CMovementComponent>();

    mRoot->SetTexture("Bullet", TEXT("Texture/block_ball.png"));
    mRoot->SetPivot(0.5f, 0.5f);
    /*mRoot->SetMesh("CenterRect");
    mRoot->SetShader("ColorMeshShader");*/

    mRoot->SetWorldScale(0.5f, 0.5f, 1.f);

    SetRootComponent(mRoot);

    mBody->SetBoxSize(50.f, 50.f);

    mBody->SetCollisionBeginFunc<CPenetrationBullet>(this,
        &CPenetrationBullet::CollisionBullet);

    mRoot->AddChild(mBody);

    mMovement->SetUpdateComponent(mRoot);

    mMovement->SetMoveAxis(EAxis::Y);
    mMovement->SetMoveSpeed(500.f);

    return true;
}

void CPenetrationBullet::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    mMoveDistance -= mMovement->GetMoveDistance();

    if (mMoveDistance <= 0.f)
        Destroy();
}

void CPenetrationBullet::CollisionBullet(const FVector3D& HitPoint,
    CColliderBase* Dest)
{
    //CLog::PrintLog("Collision");
    // 기존에 충돌된 물체여도 상관 없을 경우
    /*--mHitCount;

    if (mHitCount == 0)
        Destroy();*/

    // 기존에 충돌된 물체는 제외해야 하는 경우
    // 히트 목록에 충돌체가 있는지 검사한다.
    auto    iter = mHitList.begin();
    auto    iterEnd = mHitList.end();

    for (; iter != iterEnd; ++iter)
    {
        if (*iter == Dest)
            return;
    }

    mHitList.emplace_back(Dest);

    if ((int)mHitList.size() == mHitCount)
        Destroy();

    Dest->GetOwner()->Damage(1.f, this);
}

