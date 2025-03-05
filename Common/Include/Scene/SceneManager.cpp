#include "SceneManager.h"
#include "Scene/SceneTitle.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(mCurrentScene);
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
