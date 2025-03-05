#pragma once

#include "GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	std::vector<CSharedPtr<class CColliderBase>>	mColliderList2D;
	std::vector<CSharedPtr<class CColliderBase>>	mColliderList3D;
	class CCollisionQuadTree* mQuadTree;
	class CScene* mScene = nullptr;
	float	mInterval = 0.f;
	float	mIntervalTime = 0.f;

public:
	void AddCollider(class CColliderBase* Collider);

public:
	bool Init();
	void Update(float DeltaTime, class CSceneUIManager* UIManager,
		class CInput* Input);
	void Render();
	void ReturnNodePool();
};

