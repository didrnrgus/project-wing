#include "RotationComponent.h"
#include "SceneComponent.h"

CRotationComponent::CRotationComponent()
{
}

CRotationComponent::CRotationComponent(
    const CRotationComponent& Com) :
    CComponent(Com)
{
}

CRotationComponent::CRotationComponent(CRotationComponent&& Com) :
    CComponent(Com)
{
}

CRotationComponent::~CRotationComponent()
{
}

void CRotationComponent::SetUpdateComponent(CSceneComponent* Target)
{
    mUpdateComponent = Target;
}

bool CRotationComponent::Init()
{
    if (!CComponent::Init())
        return false;

    return true;
}

bool CRotationComponent::Init(const char* FileName)
{
    if (!CComponent::Init(FileName))
        return false;

    return true;
}

void CRotationComponent::PreUpdate(float DeltaTime)
{
    CComponent::PreUpdate(DeltaTime);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->IsActive())
            mUpdateComponent = nullptr;
    }
}

void CRotationComponent::Update(float DeltaTime)
{
    CComponent::Update(DeltaTime);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->IsActive())
            mUpdateComponent = nullptr;

        else
        {
            if (mVelocity.Length() > 0.f)
            {
                mRotationStep = mVelocity * DeltaTime;
            }

            mUpdateComponent->AddWorldRotation(mRotationStep);

            //mUpdateComponent->AddWorldPos(mMoveStep);
        }
    }
}

void CRotationComponent::PostUpdate(float DeltaTime)
{
    CComponent::PostUpdate(DeltaTime);

    if (mUpdateComponent)
    {
        if (!mUpdateComponent->IsActive())
            mUpdateComponent = nullptr;
    }
}

CRotationComponent* CRotationComponent::Clone()
{
    return new CRotationComponent(*this);
}

void CRotationComponent::EndFrame()
{
    CComponent::EndFrame();

    if (mVelocityInit)
        mVelocity = FVector3D::Zero;

    mRotationStep = FVector3D::Zero;
}

