#pragma once
#include "Object/SceneObject.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IObjectNetworkController.h"

class CObstacleGroupObject : public CSceneObject, public IGamePlayStateController, public IObjectNetworkController
{
	friend class CScene;

protected:
	CObstacleGroupObject();
	CObstacleGroupObject(const CObstacleGroupObject& Obj);
	CObstacleGroupObject(CObstacleGroupObject&& Obj);
	virtual ~CObstacleGroupObject();

protected:
	CSharedPtr<class CSceneComponent> mRoot;
	std::vector<CSharedPtr<class CSpriteComponent>> mImages;
	std::vector<CSharedPtr<class CColliderOBB2D>> mBodies;

	class IPlayerStatController* mTargetPlayerStat;

	FVector2D mResolution;
	int mReadyIndex = 0;
	const int mMaxObstacleCount = 10;

	float mCurTriggerObstacleTime;
	float mMaxTriggerObstacleTime;

	// Debug TextBlock
	CSharedPtr<class CWidgetComponent> mDebugTextComp;
	class CDebugWidget* mDebugWidget;

public:
	virtual bool Init() override;
	virtual void Update(float DeltaTime) override;

	void SetTargetStat(class IPlayerStatController* targetPlayerStat) { mTargetPlayerStat = targetPlayerStat; }
	void SetDebugText(const wchar_t* wstr);
private:
	void SetTransformImfoAndStart(float _scale, float _rot, float _height);

protected:
	virtual void AddListener() override;
	virtual void RemoveListener() override;
public:
	virtual void ProcessMessage(const struct RecvMessage& msg) override;
};

