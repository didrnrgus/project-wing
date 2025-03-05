#pragma once

#include "../Share/Object.h"

class CSceneObject :
    public CObject
{
    friend class CScene;

protected:
    CSceneObject();
    CSceneObject(const CSceneObject& Obj);
    CSceneObject(CSceneObject&& Obj);
    virtual ~CSceneObject();

protected:
	class CScene* mScene = nullptr;
	std::string		mName;
	CSharedPtr<class CSceneComponent>	mRootComponent;
	std::vector<CSharedPtr<class CComponent>>	mNonComponentList;
	std::vector<CSharedPtr<class CComponent>>	mSceneComponentList;
	float			mLifeTime = 0.f;
	bool			mDamageEnable = true;

public:
	class CSceneComponent* GetRootComponent()
	{
		return mRootComponent;
	}

	class CScene* GetScene()	const
	{
		return mScene;
	}

	const char* GetName()	const
	{
		// c_str() 함수는 string이 가지고 있는 문자열 포인터를 반환한다.
		return mName.c_str();
	}

	void SetName(const std::string& Name)
	{
		mName = Name;
	}

	void SetLifeTime(float Time)
	{
		mLifeTime = Time;
	}

	void SetDamageEnable(bool Enable)
	{
		mDamageEnable = Enable;
	}

	void SetRootComponent(class CSceneComponent* Root);

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
	virtual CSceneObject* Clone();
	virtual void Destroy();
	virtual void EndFrame();

public:
	virtual float Damage(float Attack, CSceneObject* Obj);

public:
	template <typename T>
	T* CreateComponent(const std::string& Name = "Component")
	{
		T* Component = new T;

		Component->SetName(Name);
		Component->mScene = mScene;
		Component->mOwnerObject = this;

		if (!Component->Init())
		{
			SAFE_DELETE(Component);
			return nullptr;
		}

		class CSceneComponent* Com = dynamic_cast<CSceneComponent*>(Component);

		if (!Com)
			mNonComponentList.emplace_back(Component);

		else
			mSceneComponentList.emplace_back(Component);

		return Component;
	}

	template <typename T>
	T* FindNonSceneComponent()
	{
		auto	iter = mNonComponentList.begin();
		auto	iterEnd = mNonComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			T* Com = dynamic_cast<T*>((*iter).Get());

			if (Com)
				return Com;
		}

		return nullptr;
	}

	template <typename T>
	T* FindSceneComponent()
	{
		auto	iter = mSceneComponentList.begin();
		auto	iterEnd = mSceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			T* Com = dynamic_cast<T*>((*iter).Get());

			if (Com)
				return Com;
		}

		return nullptr;
	}



public:
	const FMatrix& GetScaleMatrix()	const;

	const FMatrix& GetRotationMatrix()	const;

	const FMatrix& GetTranslateMatrix()	const;

	const FMatrix& GetWorldMatrix()	const;

	const FVector3D& GetAxis(EAxis::Type Axis)	const;

	const FVector3D& GetRelativeScale()	const;

	const FVector3D& GetRelativeRotation()	const;

	const FVector3D& GetRelativePosition()	const;

	const FVector3D& GetWorldScale()	const;

	const FVector3D& GetWorldRotation()	const;

	const FVector3D& GetWorldPosition()	const;

public:
	void SetRelativeScale(const FVector3D& Scale);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativeScale(const FVector2D& Scale);
	void SetRelativeScale(float x, float y);

	void SetRelativeRotation(const FVector3D& Rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotation(const FVector2D& Rot);
	void SetRelativeRotation(float x, float y);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);
	void SetRelativeRotationAxis(float Angle, const FVector3D& Axis);

	void SetRelativePos(const FVector3D& Pos);
	void SetRelativePos(float x, float y, float z);
	void SetRelativePos(const FVector2D& Pos);
	void SetRelativePos(float x, float y);

	void SetWorldScale(const FVector3D& Scale);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScale(const FVector2D& Scale);
	void SetWorldScale(float x, float y);

	void SetWorldRotation(const FVector3D& Rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotation(const FVector2D& Rot);
	void SetWorldRotation(float x, float y);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);
	void SetWorldRotationAxis(float Angle, const FVector3D& Axis);

	void SetWorldPos(const FVector3D& Pos);
	void SetWorldPos(float x, float y, float z);
	void SetWorldPos(const FVector2D& Pos);
	void SetWorldPos(float x, float y);

public:
	void AddRelativeScale(const FVector3D& Scale);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativeScale(const FVector2D& Scale);
	void AddRelativeScale(float x, float y);

	void AddRelativeRotation(const FVector3D& Rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotation(const FVector2D& Rot);
	void AddRelativeRotation(float x, float y);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

	void AddRelativePos(const FVector3D& Pos);
	void AddRelativePos(float x, float y, float z);
	void AddRelativePos(const FVector2D& Pos);
	void AddRelativePos(float x, float y);

	void AddWorldScale(const FVector3D& Scale);
	void AddWorldScale(float x, float y, float z);
	void AddWorldScale(const FVector2D& Scale);
	void AddWorldScale(float x, float y);

	void AddWorldRotation(const FVector3D& Rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotation(const FVector2D& Rot);
	void AddWorldRotation(float x, float y);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);

	void AddWorldPos(const FVector3D& Pos);
	void AddWorldPos(float x, float y, float z);
	void AddWorldPos(const FVector2D& Pos);
	void AddWorldPos(float x, float y);

};

