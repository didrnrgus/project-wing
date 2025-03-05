#pragma once

#include "../GameInfo.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	CSharedPtr<class CCameraComponent>	mViewTarget;
	FMatrix		mIdentity;

public:
	class CCameraComponent* GetViewTarget()	const
	{
		return mViewTarget;
	}

	const FVector3D& GetCameraWorldPos()	const;

	const FMatrix& GetViewMatrix()	const;

	const FMatrix& GetProjMatrix()	const;

public:
	void SetViewTarget(class CCameraComponent* ViewTarget);

public:
	bool Init();
	void Update(float DeltaTime);
};

