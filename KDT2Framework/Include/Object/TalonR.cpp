#include "TalonR.h"
#include "Component/StaticMeshComponent.h"
#include "Component/MovementComponent.h"
#include "Component/RotationComponent.h"
#include "Component/SpriteComponent.h"

CTalonR::CTalonR()
{
}

CTalonR::CTalonR(const CTalonR& Obj) :
    CSceneObject(Obj)
{
}

CTalonR::CTalonR(CTalonR&& Obj) :
    CSceneObject(Obj)
{
}

CTalonR::~CTalonR()
{
}

bool CTalonR::Init()
{
    mMesh = CreateComponent<CSpriteComponent>();

    mMovement = CreateComponent<CMovementComponent>();
    mRotation = CreateComponent<CRotationComponent>();

    mMovement->SetUpdateComponent(mMesh);
    mRotation->SetUpdateComponent(mMesh);

    mRotation->SetEnable(false);
    mRotation->SetVelocityInit(false);

    mRotation->SetMoveZ(1080.f);

    mMovement->SetMoveAxis(EAxis::Y);

    mMovement->SetMoveSpeed(mMaxRange / mReadyTime);

    mMesh->SetTexture("Bullet", TEXT("Texture/block_ball.png"));
    mMesh->SetPivot(0.5f, 0.5f);
    /*mMesh->SetMesh("CenterRect");
    mMesh->SetShader("ColorMeshShader");*/

    mMesh->SetWorldScale(50.f, 50.f, 1.f);

    SetRootComponent(mMesh);

    return true;
}

void CTalonR::PreUpdate(float DeltaTime)
{
    CSceneObject::PreUpdate(DeltaTime);

    FVector3D Pos = mMesh->GetWorldPosition();
    FVector3D   Dir = mMesh->GetAxis(EAxis::Y);

    mTimeAcc += DeltaTime;

    switch (mState)
    {
    case Expansion:
        mRange = DeltaTime / mReadyTime *
            mMaxRange;

        //mMesh->SetWorldPos(Pos + Dir * mRange);

        if (mTimeAcc >= mReadyTime)
        {
            mTimeAcc -= mReadyTime;
            mState = ETalonRState::Maintain;
            mRotation->SetEnable(true);
            mMovement->SetEnable(false);
        }
        break;
    case Maintain:
    {
        /*float   Angle = mMesh->GetWorldRotation().z;

        Angle += 1080.f * DeltaTime;

        mMesh->SetWorldRotationZ(Angle);*/

        if (mTimeAcc >= mTime)
        {
            mTimeAcc = 0.f;
            mState = ETalonRState::Reduction;
            SetLifeTime(5.f);
            mRotation->SetEnable(false);
            mMovement->SetEnable(true);

            mMovement->SetMoveAxis(EAxis::None);
        }
    }
    break;
    case Reduction:
    {
        Dir = mTarget->GetRootComponent()->GetWorldPosition() -
            mMesh->GetWorldPosition();
        Dir.Normalize();

        mMovement->SetMove(Dir);

        //mMesh->SetWorldPos(Pos + Dir * 10.f * DeltaTime);
    }
    break;
    }
}

void CTalonR::Update(float DeltaTime)
{
    CSceneObject::Update(DeltaTime);
}
