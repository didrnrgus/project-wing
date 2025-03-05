#pragma once

#include "SceneComponent.h"

class CMeshComponent :
    public CSceneComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent& Com);
	CMeshComponent(CMeshComponent&& Com);
	virtual ~CMeshComponent();

protected:
	std::vector<CSharedPtr<class CMaterial>>	mMaterialSlots;

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual CMeshComponent* Clone();
};

