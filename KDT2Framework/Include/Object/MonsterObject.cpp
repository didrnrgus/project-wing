#include "MonsterObject.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "BulletObject.h"
#include "../Component/ColliderAABB2D.h"
#include "../Component/ColliderOBB2D.h"
#include "../Component/ColliderSphere2D.h"
#include "Share/Log.h"
#include "Component/SpriteComponent.h"

CMonsterObject::CMonsterObject()
{
}

CMonsterObject::CMonsterObject(const CMonsterObject& Obj)   :
    CSceneObject(Obj)
{
}

CMonsterObject::CMonsterObject(CMonsterObject&& Obj) :
    CSceneObject(Obj)
{
}

CMonsterObject::~CMonsterObject()
{
}

bool CMonsterObject::Init()
{
    //mRoot = CreateComponent<CStaticMeshComponent>();
    mRoot = CreateComponent<CSpriteComponent>();
    //mBody = CreateComponent<CColliderAABB2D>();
    mBody = CreateComponent<CColliderSphere2D>();
    //mBody = CreateComponent<CColliderOBB2D>();
    mDetect = CreateComponent<CColliderSphere2D>();

    //mRoot->SetMesh("CenterRect");
    //mRoot->SetShader("ColorMeshShader");

    mRoot->SetWorldScale(100.f, 100.f, 1.f);

    SetRootComponent(mRoot);

    mBody->SetCollisionProfile("Monster");
    mBody->SetRadius(50.f);
    //mBody->SetBoxSize(100.f, 100.f);
    mBody->SetCollisionBeginFunc<CMonsterObject>(this,
        &CMonsterObject::CollisionMonster);
    mBody->SetCollisionEndFunc<CMonsterObject>(this,
        &CMonsterObject::CollisionMonsterEnd);

    mRoot->AddChild(mBody);

    mDetect->SetCollisionProfile("MonsterDetect");
    mDetect->SetRadius(500.f);
    //mBody->SetBoxSize(100.f, 100.f);
    mDetect->SetCollisionBeginFunc<CMonsterObject>(this,
        &CMonsterObject::CollisionMonsterDetect);
    mDetect->SetCollisionEndFunc<CMonsterObject>(this,
        &CMonsterObject::CollisionMonsterDetectEnd);

    mRoot->AddChild(mDetect);

    return true;
}

void CMonsterObject::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);

    if (mTarget)
    {
        if (!mTarget->IsActive())
        {
            mAI = EMonsterAI::Idle;
            mTarget = nullptr;
        }
    }

    switch (mAI)
    {
    case EMonsterAI::Idle:
        AIIdle();
        break;
    case EMonsterAI::Trace:
        AITrace();
        break;
    case EMonsterAI::Patrol:
        AIPatrol();
        break;
    case EMonsterAI::Attack:
        AIAttack();
        break;
    case EMonsterAI::Death:
        AIDeath();
        break;
    case EMonsterAI::Skill:
        AISkill();
        break;
    case EMonsterAI::Custom:
        AICustom();
        break;
    }
}

float CMonsterObject::Damage(float Attack,
    CSceneObject* Obj)
{
    float Dmg = CSceneObject::Damage(Attack, Obj);

    mHP -= (int)Dmg;

    if (mHP <= 0)
        Destroy();

    return Dmg;
}

void CMonsterObject::DetectTarget()
{
}

void CMonsterObject::DetectTargetRelease()
{
}

void CMonsterObject::CollisionMonster(
    const FVector3D& HitPoint, CColliderBase* Dest)
{
    //CLog::PrintLog("Collision");
}

void CMonsterObject::CollisionMonsterEnd(
    CColliderBase* Dest)
{
    //CLog::PrintLog("CollisionEnd");
}

void CMonsterObject::CollisionMonsterDetect(const FVector3D& HitPoint, 
    CColliderBase* Dest)
{
    mTarget = Dest->GetOwner();
    mAI = EMonsterAI::Trace;
    DetectTarget();
}

void CMonsterObject::CollisionMonsterDetectEnd(CColliderBase* Dest)
{
    mTarget = nullptr;
    mAI = EMonsterAI::Idle;
    DetectTargetRelease();
}

void CMonsterObject::AIIdle()
{
    if (mAnimation)
        mAnimation->ChangeAnimation(mAIAnimationName[(int)EMonsterAI::Idle]);
}

void CMonsterObject::AIPatrol()
{
}

void CMonsterObject::AITrace()
{
}

void CMonsterObject::AIAttack()
{
    if (mAnimation)
        mAnimation->ChangeAnimation(mAIAnimationName[(int)EMonsterAI::Attack]);
}

void CMonsterObject::AIDeath()
{
}

void CMonsterObject::AISkill()
{
    if (mAnimation)
        mAnimation->ChangeAnimation(mAIAnimationName[(int)EMonsterAI::Skill]);
}

void CMonsterObject::AICustom()
{
}
