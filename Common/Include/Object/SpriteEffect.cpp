#include "SpriteEffect.h"
#include "../Component/SpriteComponent.h"
#include "../Component/AudioComponent.h"
#include "../Animation/Animation2D.h"

CSpriteEffect::CSpriteEffect()
{
}

CSpriteEffect::CSpriteEffect(const CSpriteEffect& Obj)  :
    CSceneObject(Obj)
{
}

CSpriteEffect::CSpriteEffect(CSpriteEffect&& Obj) :
    CSceneObject(Obj)
{
}

CSpriteEffect::~CSpriteEffect()
{
}

bool CSpriteEffect::Init()
{
    CSceneObject::Init();

    mRoot = CreateComponent<CSpriteComponent>();
    mAudio = CreateComponent<CAudioComponent>();

    SetRootComponent(mRoot);

    mRoot->SetPivot(0.5f, 0.5f);

    mRoot->AddChild(mAudio);

    mAudio->SetSound("Hit");

    mAnimation = mRoot->CreateAnimation2D<CAnimation2D>();

    return true;
}

void CSpriteEffect::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);
}

void CSpriteEffect::SetAnimation(const std::string& Name, float PlayTime,
    float PlayRate, bool Loop, bool Reverse)
{
    mLoop = Loop;

    mAnimation->AddSequence(Name, PlayTime, PlayRate, Loop, Reverse);

    mAnimation->SetEndFunction<CSpriteEffect>(Name, this,
        &CSpriteEffect::AnimationEnd);
}

void CSpriteEffect::AnimationEnd()
{
    if (!mLoop)
        Destroy();
}
