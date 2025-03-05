#include "AudioComponent.h"
#include "Asset/AssetManager.h"
#include "Asset/Sound/Sound.h"
#include "Asset/Sound/SoundManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"

CAudioComponent::CAudioComponent()
{
}

CAudioComponent::CAudioComponent(const CAudioComponent& Com) :
    CSceneComponent(Com)
{
}

CAudioComponent::CAudioComponent(CAudioComponent&& Com) :
    CSceneComponent(Com)
{
}

CAudioComponent::~CAudioComponent()
{
    /*if (mSound)
        mSound->Stop();*/
}

void CAudioComponent::SetSound(const std::string& Name)
{
    if (mScene)
        mSound = mScene->GetAssetManager()->FindSound(Name);

    else
        mSound = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);

    if (mSound)
        mSound->Play();
}

void CAudioComponent::SetSound(const std::string& Name,
    const std::string& GroupName, bool Loop, const char* FileName)
{
    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadSound(Name, GroupName,
            Loop, FileName))
        {
            return;
        }
        mSound = mScene->GetAssetManager()->FindSound(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetSoundManager()->LoadSound(Name,
            GroupName, Loop, FileName))
        {
            return;
        }
        mSound = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
    }

    if (mSound)
        mSound->Play();
}

void CAudioComponent::SetSoundFullPath(const std::string& Name,
    const std::string& GroupName, bool Loop, const char* FullPath)
{
    if (mScene)
    {
        if (!mScene->GetAssetManager()->LoadSoundFullPath(Name, GroupName,
            Loop, FullPath))
        {
            return;
        }
        mSound = mScene->GetAssetManager()->FindSound(Name);
    }

    else
    {
        if (!CAssetManager::GetInst()->GetSoundManager()->LoadSoundFullPath(Name,
            GroupName, Loop, FullPath))
        {
            return;
        }
        mSound = CAssetManager::GetInst()->GetSoundManager()->FindSound(Name);
    }

    if (mSound)
        mSound->Play();
}

bool CAudioComponent::Init()
{
    CSceneComponent::Init();

    return true;
}

bool CAudioComponent::Init(const char* FileName)
{
    CSceneComponent::Init(FileName);

    return true;
}

void CAudioComponent::Update(float DeltaTime)
{
    CSceneComponent::Update(DeltaTime);
}

CAudioComponent* CAudioComponent::Clone()
{
    return new CAudioComponent(*this);
}
