#include "CameraObject.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/Input.h"

CCameraObject::CCameraObject()
{
}

CCameraObject::CCameraObject(const CCameraObject& Obj)
{
}

CCameraObject::CCameraObject(CCameraObject&& Obj)
{
}

CCameraObject::~CCameraObject()
{
}

bool CCameraObject::Init()
{
	CSceneObject::Init();

	mCamera = CreateComponent<CCameraComponent>();
	mCamera->SetProjectionType(ECameraProjectionType::Ortho);
	mCamera->SetWorldPos(FVector3D::Zero);
	SetRootComponent(mCamera);

	mScene->GetInput()->AddBindKey("TestShake", 'S');
	mScene->GetInput()->AddBindFunction("TestShake", EInputType::Down
		, [this](float DeltaTime)
		{
			CLog::PrintLog("mScene->GetInput()->AddBindFunction TestShake");
			SetShakeSceneObject(2.0f, 10.0f);
		});

    return true;
}

void CCameraObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);

	if (GetIsShake())
	{
		ReleaseShake(DeltaTime);
	}
}

void CCameraObject::SetShakeSceneObject(float duration, float intensity)
{
	IGamePlayShakeController::SetShakeSceneObject(duration, intensity);

}

