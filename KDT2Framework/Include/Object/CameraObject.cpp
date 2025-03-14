#include "CameraObject.h"
#include "Component/CameraComponent.h"

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

    return true;
}

void CCameraObject::Update(float DeltaTime)
{
	CSceneObject::Update(DeltaTime);
}
