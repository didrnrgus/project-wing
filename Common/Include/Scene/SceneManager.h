#pragma once

#include "../GameInfo.h"

class CSceneManager
{
private:
	class CScene* mCurrentScene = nullptr;
	class CScene* mLoadScene = nullptr;

public:
	class CScene* GetCurrentScene()	const
	{
		return mCurrentScene;
	}

public:
	bool Init();
	void Input(float DeltaTime);
	bool Update(float DeltaTime);
	void Collision(float DeltaTime);
	void Render();
	void RenderUI();
	void EndFrame();

public:
	template <typename T>
	T* CreateScene()
	{
		T* Scene = new T;

		if (!Scene->Init())
		{
			SAFE_DELETE(Scene);
			return nullptr;
		}

		return Scene;
	}

	template <typename T>
	T* CreateLoadScene()
	{
		T* Scene = new T;

		if (!Scene->Init())
		{
			SAFE_DELETE(Scene);
			return nullptr;
		}

		mLoadScene = Scene;

		return Scene;
	}

	DECLARE_SINGLE(CSceneManager)
};

