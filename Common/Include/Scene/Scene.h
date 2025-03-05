#pragma once

#include "../GameInfo.h"
#include "../Object/SceneObject.h"

class CScene abstract
{
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

protected:
	class CInput* mInput = nullptr;
	class CCameraManager* mCameraManager = nullptr;
	class CSceneCollision* mCollision = nullptr;
	class CSceneAssetManager* mAssetManager = nullptr;
	class CSceneUIManager* mUIManager = nullptr;
	std::list<CSharedPtr<class CSceneObject>>	mObjList;
	bool	mDebugQuadTree = true;

public:
	class CSceneUIManager* GetUIManager()	const
	{
		return mUIManager;
	}

	class CInput* GetInput()	const
	{
		return mInput;
	}

	class CCameraManager* GetCameraManager()	const
	{
		return mCameraManager;
	}

	class CSceneCollision* GetCollision()	const
	{
		return mCollision;
	}

	class CSceneAssetManager* GetAssetManager()	const
	{
		return mAssetManager;
	}

public:
	virtual bool Init();
	virtual bool Init(const char* FileName);
	virtual void Input(float DeltaTime);
	virtual void PreUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render();
	virtual void RenderUI();
	virtual void EndFrame();

protected:
	virtual bool InitAsset() = 0;
	virtual bool InitObject() = 0;
	virtual bool InitWidget() = 0;

public:
	template <typename T>
	T* CreateObj(const std::string& Name)
	{
		T* Obj = new T;

		Obj->mScene = this;
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		mObjList.push_back(Obj);

		return Obj;
	}

	template <typename T>
	T* FindObjectFromType()
	{
		auto	iter = mObjList.begin();
		auto	iterEnd = mObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			T* Obj = dynamic_cast<T*>((*iter).Get());

			if (Obj)
				return Obj;
		}

		return nullptr;
	}

	template <typename T>
	T* FindObjectFromName(const std::string& Name)
	{
		auto	iter = mObjList.begin();
		auto	iterEnd = mObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return dynamic_cast<T*>((*iter).Get());
		}

		return nullptr;
	}

	template <typename T>
	void FindObjectsFromType(
		std::list<CSharedPtr<T>>& result)
	{
		auto	iter = mObjList.begin();
		auto	iterEnd = mObjList.end();

		for (; iter != iterEnd; ++iter)
		{
			T* Obj = dynamic_cast<T*>((*iter).Get());

			if (Obj)
				result.push_back(Obj);
		}
	}
};

