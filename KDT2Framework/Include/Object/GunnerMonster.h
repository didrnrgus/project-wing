#pragma once
#include "MonsterObject.h"
class CGunnerMonster :
    public CMonsterObject
{
    friend class CScene;

protected:
    CGunnerMonster();
    CGunnerMonster(const CGunnerMonster& Obj);
    CGunnerMonster(CGunnerMonster&& Obj);
    virtual ~CGunnerMonster();

protected:
    float       mFireTime = 1.f;
    int         mFireCount = 0;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

protected:
    virtual void DetectTarget();

protected:
    virtual void AIAttack();

protected:
    void AttackNotify();
    void AttackEnd();
    void SkillNotify();
    void SkillEnd();
};

