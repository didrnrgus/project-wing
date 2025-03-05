#pragma once

#include "SceneComponent.h"

class CAudioComponent :
    public CSceneComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CAudioComponent();
	CAudioComponent(const CAudioComponent& Com);
	CAudioComponent(CAudioComponent&& Com);
	virtual ~CAudioComponent();

protected:
	CSharedPtr<class CSound>	mSound;

public:
	void SetSound(const std::string& Name);
	void SetSound(const std::string& Name, const std::string& GroupName,
		bool Loop, const char* FileName);
	void SetSoundFullPath(const std::string& Name, const std::string& GroupName,
		bool Loop, const char* FullPath);

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void Update(float DeltaTime);
	virtual CAudioComponent* Clone();
};

