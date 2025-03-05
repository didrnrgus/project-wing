#pragma once

#include "Object/SceneObject.h"

enum class EMonsterAI : unsigned char
{
    Idle,
    Trace,
    Patrol,
    Attack,
    Death,
    Skill,
    Custom,
    End
};

class CMonsterObject :
    public CSceneObject
{
    friend class CScene;

protected:
    CMonsterObject();
    CMonsterObject(const CMonsterObject& Obj);
    CMonsterObject(CMonsterObject&& Obj);
    virtual ~CMonsterObject();

protected:
    CSharedPtr<class CSpriteComponent>  mRoot;
    //CSharedPtr<class CStaticMeshComponent>  mRoot;
    //CSharedPtr<class CColliderAABB2D>       mBody;
    CSharedPtr<class CColliderSphere2D>       mBody;
    //CSharedPtr<class CColliderOBB2D>       mBody;
    CSharedPtr<class CColliderSphere2D>       mDetect;
    CSharedPtr<CSceneObject>  mTarget;
    class CAnimation2D* mAnimation = nullptr;
    EMonsterAI          mAI = EMonsterAI::Idle;
    std::string         mAIAnimationName[(int)EMonsterAI::End];

    float           mDetectDistance = 0.f;

    int             mHP = 5;

public:
    void SetTarget(class CSceneObject* Target)
    {
        mTarget = Target;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    virtual float Damage(float Attack, CSceneObject* Obj);

protected:
    virtual void DetectTarget();
    virtual void DetectTargetRelease();

private:
    void CollisionMonster(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    void CollisionMonsterEnd(class CColliderBase* Dest);

    void CollisionMonsterDetect(const FVector3D& HitPoint,
        class CColliderBase* Dest);
    void CollisionMonsterDetectEnd(class CColliderBase* Dest);

protected:  // ============ AI Virtual Function ===============
    virtual void AIIdle();
    virtual void AIPatrol();
    virtual void AITrace();
    virtual void AIAttack();
    virtual void AIDeath();
    virtual void AISkill();
    virtual void AICustom();
};

