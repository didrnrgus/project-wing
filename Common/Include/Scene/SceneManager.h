#pragma once

#include "../GameInfo.h"

class CSceneManager
{
private:
	class CScene* mCurrentScene = nullptr;
	class CScene* mLoadScene = nullptr;
	EGameScene::Type mSceneType = EGameScene::End;

public:
	class CScene* GetCurrentScene()	const
	{
		return mCurrentScene;
	}

	EGameScene::Type GetCurrentSceneType() const
	{
		return mSceneType;
	}

	EGameScene::Type SetCurrentSceneType(class CScene* scene);

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


