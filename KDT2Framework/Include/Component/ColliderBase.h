#pragma once

#include "Component/SceneComponent.h"

class CColliderBase abstract :
	public CSceneComponent
{
	friend class CScene;
	friend class CSceneObject;

protected:
	CColliderBase();
	CColliderBase(const CColliderBase& Com);
	CColliderBase(CColliderBase&& Com);
	virtual ~CColliderBase();

protected:
	std::unordered_map<CColliderBase*, bool>	mCollisionObjects;

protected:
	EColliderType	mColliderType;
	EColliderShape	mColliderShape;
	FVector3D		mMin;
	FVector3D		mMax;
	FCollisionProfile* mProfile = nullptr;
	bool			mCollision = false;
	std::function<void(const FVector3D&, CColliderBase*)>	mCollisionBeginFunc;
	class CSceneObject* mBeginObj;
	std::function<void(CColliderBase*)>	mCollisionEndFunc;
	class CSceneObject* mEndObj;

#ifdef _DEBUG

	class CColliderCBuffer* mCBuffer = nullptr;
	CSharedPtr<class CMesh> mMesh;
	CSharedPtr<class CShader> mShader;
	bool		mEnableRotation = false;

#endif // _DEBUG


public:
	FCollisionProfile* GetProfile()	const
	{
		return mProfile;
	}

	EColliderShape GetColliderShape()	const
	{
		return mColliderShape;
	}

	EColliderType GetColliderType()	const
	{
		return mColliderType;
	}

	const FVector3D& GetMin()
	{
		return mMin;
	}

	const FVector3D& GetMax()
	{
		return mMax;
	}

public:
	void SetCollisionProfile(const std::string& Name);
	void CallCollisionBegin(const FVector3D& HitPoint,
		CColliderBase* Dest);
	void CallCollisionEnd(CColliderBase* Dest);
	bool CheckCollisionObject(CColliderBase* Collider);
	void AddCollisionObject(CColliderBase* Collider);
	void EraseCollisionObject(CColliderBase* Collider);

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
	virtual CColliderBase* Clone();
	virtual void EraseOwner();
	virtual bool Collision(FVector3D& HitPoint, CColliderBase* Dest) = 0;

public:
	template <typename T>
	void SetCollisionBeginFunc(T* Obj, void (T::* Func)(const FVector3D&, CColliderBase*))
	{
		mBeginObj = Obj;
		mCollisionBeginFunc = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

	using T0 = std::function<void(const FVector3D&, CColliderBase*)>;
	void SetCollisionBeginFunc(T0&& Func)
	{
		mCollisionBeginFunc = std::move(Func);
	}

	void ClearBeginFunction()
	{
		mCollisionBeginFunc = nullptr;
	}

	template <typename T>
	void SetCollisionEndFunc(T* Obj, void (T::* Func)(CColliderBase*))
	{
		mEndObj = Obj;
		mCollisionEndFunc = std::bind(Func, Obj, std::placeholders::_1);
	}

	using T1 = std::function<void(CColliderBase*)>;
	void SetCollisionEndFunc(T1&& Func)
	{
		mCollisionEndFunc = std::move(Func);
	}

	void ClearEndFunction()
	{
		mCollisionEndFunc = nullptr;
	}
};

