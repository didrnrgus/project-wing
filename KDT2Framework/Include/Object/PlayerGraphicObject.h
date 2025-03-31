#pragma once
#include "Object/SceneObject.h"

class CPlayerGraphicObject : public CSceneObject
{
	friend class CScene;

protected:
	CPlayerGraphicObject();
	CPlayerGraphicObject(const CPlayerGraphicObject& Obj);
	CPlayerGraphicObject(CPlayerGraphicObject&& Obj);
	virtual ~CPlayerGraphicObject();

private:
	float mMapDifficultySinAngle;

protected:
	CSharedPtr<class CSpriteComponent> mRoot;
	class CAnimation2D* mAnimation = nullptr;
	FVector3D mRootInitPos;
	FVector2D mResolution;

	bool mIsMine = false;
	class IGamePlayShakeController* mCameraShake;
	
public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

public:
	bool SetChangeGraphic(int index);
	// 씬에서 호출.
	inline void SetIsMine(bool isMine) { mIsMine = isMine; }
	inline bool GetIsMine() { return mIsMine; }
	
	inline void SetShakeCamera(class IGamePlayShakeController* cameraShake)
	{
		mCameraShake = cameraShake;
	}
};
