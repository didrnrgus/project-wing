#pragma once

#include "GameInfo.h"

#define	QUADTREE_DIVISION_COUNT	5
#define	QUADTREE_DEPTH_MAX		4

class CCollisionQuadTreeNode
{
	friend class CCollisionQuadTree;

private:
	CCollisionQuadTreeNode();
	~CCollisionQuadTreeNode();

private:
	class CScene* mScene = nullptr;
	CCollisionQuadTree* mTree = nullptr;
	CCollisionQuadTreeNode* mParent = nullptr;
	CCollisionQuadTreeNode* mChild[4] = {};
	FVector2D		mCenter;
	FVector2D		mSize;
	int				mDepth = 0;
	int				mDivisionCount = QUADTREE_DIVISION_COUNT;
	std::vector<CSharedPtr<class CColliderBase>>	mColliderList;

#ifdef _DEBUG
	class CTransformCBuffer* mTransformCBuffer;
#endif // _DEBUG


public:
	void AddCollider(class CColliderBase* Collider,
		std::vector<CCollisionQuadTreeNode*>& NodePool);
	void CreateChild(std::vector<CCollisionQuadTreeNode*>& NodePool);
	void Collision(float DeltaTime);
	void ReturnNodePool(std::vector<CCollisionQuadTreeNode*>& NodePool);
	void Render(class CMesh* Mesh, class CShader* Shader);

private:
	bool IsInCollider(class CColliderBase* Collider);
};

class CCollisionQuadTree
{
	friend class CSceneCollision;

private:
	CCollisionQuadTree();
	~CCollisionQuadTree();

private:
	class CScene* mScene = nullptr;
	CCollisionQuadTreeNode* mRoot = nullptr;
	int		mDivisionCount = QUADTREE_DIVISION_COUNT;
	std::vector<CCollisionQuadTreeNode*>	mNodePool;
	std::vector<CCollisionQuadTreeNode*>	mCollisionNodeList;

#ifdef _DEBUG

	CSharedPtr<class CMesh>		mMesh;
	CSharedPtr<class CShader>	mShader;
	class CColliderCBuffer* mColliderCBuffer;

#endif // _DEBUG


public:
	void SetDivisionCount(int Count);
	void AddCollisionNodeList(CCollisionQuadTreeNode* Node)
	{
		

		mCollisionNodeList.emplace_back(Node);
	}

	void EraseCollisionNodeList(CCollisionQuadTreeNode* Node);

public:
	bool Init();
	void AddCollider(class CColliderBase* Collider);
	void Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render();
	void ReturnNodePool();
};

