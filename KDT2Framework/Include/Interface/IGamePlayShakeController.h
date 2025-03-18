#pragma once

#include "GameInfo.h"
#include "Object/SceneObject.h"

class IGamePlayShakeController abstract
{
private:
	bool isShake;
	float shakeDuration;
	float shakeIntensity;
	CSceneObject* shakeSceneObject;
	std::vector<FVector3D> arrShakePos =
	{
		FVector3D::Axis[EAxis::X] * -1.0f,
		FVector3D::Axis[EAxis::Y] * 1.0f,
		FVector3D::Axis[EAxis::X] * 1.0f,
		FVector3D::Axis[EAxis::Y] * -1.0f,
	};
	int shakePosIndex;
	FVector3D shakeInitPos;

public:
	virtual void SetShakeSceneObject(float duration, float intensity)
	{
		shakeSceneObject = dynamic_cast<CSceneObject*>(this);
		if (shakeSceneObject == nullptr)
		{
			CLog::PrintLog("IGamePlayShakeController::SetShakeSceneObject() ERROR!!!");
			return;
		}

		isShake = true;
		shakePosIndex = 0;
		shakeDuration = duration;
		shakeIntensity = intensity;
		shakeInitPos = shakeSceneObject->GetWorldPosition();
	}

	virtual void ReleaseShake(float DeltaTime)
	{
		shakeDuration -= DeltaTime;
		if (shakeDuration < 0.0f)
		{
			isShake = false;
			shakeSceneObject->SetWorldPos(shakeInitPos);
			return;
		}
		shakeSceneObject->SetRelativePos(arrShakePos[shakePosIndex] * shakeIntensity);
		shakePosIndex++;
		if (shakePosIndex >= arrShakePos.size())
		{
			shakePosIndex = 0;
		}
	}

	bool GetIsShake() { return isShake; }
};