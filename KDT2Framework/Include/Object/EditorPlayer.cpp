#include "EditorPlayer.h"
#include "Component/CameraComponent.h"
#include "Component/MovementComponent.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"

CEditorPlayer::CEditorPlayer()
{
}

CEditorPlayer::CEditorPlayer(const CEditorPlayer& Obj)  :
    CSceneObject(Obj)
{
}

CEditorPlayer::CEditorPlayer(CEditorPlayer&& Obj) :
    CSceneObject(Obj)
{
}

CEditorPlayer::~CEditorPlayer()
{
}

bool CEditorPlayer::Init()
{
    CSceneObject::Init();

    mCamera = CreateComponent<CCameraComponent>();

    mCamera->SetProjectionType(ECameraProjectionType::Ortho);

    SetRootComponent(mCamera);

    mMovement = CreateComponent<CMovementComponent>();

    mMovement->SetUpdateComponent(mCamera);
    mMovement->SetMoveSpeed(500.f);

    mScene->GetInput()->AddBindKey("MoveUp", 'W');
    mScene->GetInput()->AddBindKey("MoveDown", 'S');
    mScene->GetInput()->AddBindKey("MoveLeft", 'A');
    mScene->GetInput()->AddBindKey("MoveRight", 'D');

    mScene->GetInput()->AddBindFunction<CEditorPlayer>("MoveUp",
        EInputType::Hold, this, &CEditorPlayer::MoveUp);

    mScene->GetInput()->AddBindFunction<CEditorPlayer>("MoveDown",
        EInputType::Hold, this, &CEditorPlayer::MoveDown);

    mScene->GetInput()->AddBindFunction<CEditorPlayer>("MoveLeft",
        EInputType::Hold, this, &CEditorPlayer::MoveLeft);

    mScene->GetInput()->AddBindFunction<CEditorPlayer>("MoveRight",
        EInputType::Hold, this, &CEditorPlayer::MoveRight);

    return true;
}

void CEditorPlayer::MoveUp(float DeltaTime)
{
    mMovement->AddMove(mRootComponent->GetAxis(EAxis::Y));
}

void CEditorPlayer::MoveDown(float DeltaTime)
{
    mMovement->AddMove(mRootComponent->GetAxis(EAxis::Y) * -1.f);
}

void CEditorPlayer::MoveLeft(float DeltaTime)
{
    mMovement->AddMove(mRootComponent->GetAxis(EAxis::X) * -1.f);
}

void CEditorPlayer::MoveRight(float DeltaTime)
{
    mMovement->AddMove(mRootComponent->GetAxis(EAxis::X));
}
