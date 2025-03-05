#include "CameraManager.h"
#include "../Component/CameraComponent.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

const FVector3D& CCameraManager::GetCameraWorldPos() const
{
	if (!mViewTarget)
		return FVector3D::Zero;

	return mViewTarget->GetWorldPosition();
}

const FMatrix& CCameraManager::GetViewMatrix() const
{
	if (!mViewTarget)
		return mIdentity;

	return mViewTarget->GetViewMatrix();
}

const FMatrix& CCameraManager::GetProjMatrix() const
{
	if (!mViewTarget)
		return mIdentity;

	return mViewTarget->GetProjMatrix();
}

void CCameraManager::SetViewTarget(CCameraComponent* ViewTarget)
{
	mViewTarget = ViewTarget;
}

bool CCameraManager::Init()
{
	mIdentity = FMatrix::StaticIdentity();

	return true;
}

void CCameraManager::Update(float DeltaTime)
{
	if (mViewTarget)
	{
		if (!mViewTarget->IsActive())
			mViewTarget = nullptr;
	}
}
