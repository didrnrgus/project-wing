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
	CSharedPtr<class CSpriteComponent> mDeadSign;

	class CAnimation2D* mAnimation = nullptr;
	FVector3D mRootInitPos;
	FVector2D mResolution;

	bool mIsMine = false;
	int mIndexInScene = -1;
	class IGamePlayShakeController* mCameraShake;

	FVector3D mDestPos;
	
	// Debug TextBlock
	CSharedPtr<class CWidgetComponent> mDebugTextComp;
	class CDebugWidget* mDebugWidget;
	CSharedPtr<class CWidgetComponent> mPlayerObjectWidgetComp;
	class CPlayerObjectWidget* mPlayerObjectWidget;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

public:
	void SetMovePlayer(FVector3D _worldPos);
	bool SetChangeGraphic(int index);
	inline void SetIsMine(bool isMine) { mIsMine = isMine; }
	inline bool GetIsMine() { return mIsMine; }
	inline void SetShakeCamera(class IGamePlayShakeController* cameraShake)
	{
		mCameraShake = cameraShake;
	}

	inline void SetIndexInScene(int _index) { mIndexInScene = _index; }
	inline int GetIndexInScene() { return mIndexInScene; }
	void SetDebugText(const wchar_t* wstr);

	FVector3D GetDestWorldPos() { return mDestPos; }
};
