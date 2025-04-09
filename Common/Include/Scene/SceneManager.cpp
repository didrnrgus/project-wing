#include "SceneManager.h"
#include "Scene/SceneTitle.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneInGame.h"
#include "Scene/SceneResult.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(mCurrentScene);
}

EGameScene::Type CSceneManager::SetCurrentSceneType(CScene* scene)
{
	if (dynamic_cast<CSceneTitle*>(scene))
		mSceneType = EGameScene::Title;
	else if (dynamic_cast<CSceneLobby*>(scene))
		mSceneType = EGameScene::Lobby;
	else if (dynamic_cast<CSceneInGame*>(scene))
		mSceneType = EGameScene::InGame;
	else if (dynamic_cast<CSceneResult*>(scene))
		mSceneType = EGameScene::Result;
	else
		mSceneType = EGameScene::End;

	return mSceneType;
}

bool CSceneManager::Init()
{
	mCurrentScene = CreateScene<CSceneTitle>();
	return true;
}

void CSceneManager::Input(float DeltaTime)
{
	mCurrentScene->Input(DeltaTime);
}

bool CSceneManager::Update(float DeltaTime)
{
	mCurrentScene->PreUpdate(DeltaTime);

	mCurrentScene->Update(DeltaTime);

	mCurrentScene->PostUpdate(DeltaTime);

	if (mLoadScene)
	{
		// 기존 장면을 제거한다.
		SAFE_DELETE(mCurrentScene);

		mCurrentScene = mLoadScene;

		mLoadScene = nullptr;

		return true;
	}

	return false;
}

void CSceneManager::Collision(float DeltaTime)
{
	mCurrentScene->Collision(DeltaTime);
}

void CSceneManager::Render()
{
	mCurrentScene->Render();
}

void CSceneManager::RenderUI()
{
	mCurrentScene->RenderUI();
}

void CSceneManager::EndFrame()
{
	mCurrentScene->EndFrame();
}
