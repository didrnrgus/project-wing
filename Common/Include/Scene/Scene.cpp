#include "Scene.h"
#include "../Object/SceneObject.h"
#include "Input.h"
#include "CameraManager.h"
#include "Scene/SceneCollision.h"
#include "../Share/Log.h"
#include "SceneAssetManager.h"
#include "../Render/RenderManager.h"
#include "SceneUIManager.h"
#include "Scene/SceneManager.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	//CRenderManager::GetInst()->ClearRenderList();
	std::list<CSharedPtr<CSceneObject>>::iterator	iter;
	std::list<CSharedPtr<CSceneObject>>::iterator	iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Destroy();
	}

	mObjList.clear();
	SAFE_DELETE(mUIManager);
	SAFE_DELETE(mAssetManager);
	SAFE_DELETE(mCollision);
	SAFE_DELETE(mCameraManager);
	SAFE_DELETE(mInput);
}

bool CScene::Init()
{
	CSceneManager::GetInst()->SetCurrentSceneType(this);

	mInput = new CInput;

	mInput->mScene = this;

	if (!mInput->Init())
		return false;

	mCameraManager = new CCameraManager;

	if (!mCameraManager->Init())
		return false;

	mCollision = new CSceneCollision;

	mCollision->mScene = this;

	if (!mCollision->Init())
		return false;

	mAssetManager = new CSceneAssetManager;

	mAssetManager->mScene = this;

	if (!mAssetManager->Init())
		return false;

	mUIManager = new CSceneUIManager;

	mUIManager->mScene = this;

	if (!mUIManager->Init())
		return false;

	InitAsset();
	InitWidget();
	InitObject();

	return true;
}

void CScene::Input(float DeltaTime)
{
	mInput->Update(DeltaTime);
}

void CScene::PreUpdate(float DeltaTime)
{
	std::list<CSharedPtr<CSceneObject>>::iterator	iter;
	std::list<CSharedPtr<CSceneObject>>::iterator	iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// erase를 하면 지운 iterator의 다음 iterator를 반환한다.
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PreUpdate(DeltaTime);

		++iter;
	}
}

void CScene::Update(float DeltaTime)
{
	std::list<CSharedPtr<CSceneObject>>::iterator	iter;
	std::list<CSharedPtr<CSceneObject>>::iterator	iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// erase를 하면 지운 iterator의 다음 iterator를 반환한다.
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);

		++iter;
	}

	mCameraManager->Update(DeltaTime);

	mUIManager->Update(DeltaTime);

	int	Count = (int)mObjList.size();
	
	char	Text[64] = {};
	sprintf_s(Text, "ObjCount : %d", Count);

	//CLog::PrintLog(Text);
}

void CScene::PostUpdate(float DeltaTime)
{
	std::list<CSharedPtr<CSceneObject>>::iterator	iter;
	std::list<CSharedPtr<CSceneObject>>::iterator	iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// erase를 하면 지운 iterator의 다음 iterator를 반환한다.
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);

		++iter;
	}
}

void CScene::Collision(float DeltaTime)
{
	// UI와의 충돌도 Collision 안쪽에서 처리한다.
	mCollision->Update(DeltaTime, mUIManager, mInput);
}

void CScene::Render()
{
#ifdef _DEBUG

	if (mDebugQuadTree)
		mCollision->Render();

#endif // _DEBUG

	mCollision->ReturnNodePool();

	//std::list<CSharedPtr<CSceneObject>>::iterator	iter;
	//std::list<CSharedPtr<CSceneObject>>::iterator	iterEnd = mObjList.end();

	//for (iter = mObjList.begin(); iter != iterEnd;)
	//{
	//	if (!(*iter)->IsActive())
	//	{
	//		// erase를 하면 지운 iterator의 다음 iterator를 반환한다.
	//		iter = mObjList.erase(iter);
	//		iterEnd = mObjList.end();
	//		continue;
	//	}

	//	else if (!(*iter)->IsEnable())
	//	{
	//		++iter;
	//		continue;
	//	}

	//	(*iter)->Render();

	//	++iter;
	//}

}

void CScene::RenderUI()
{
	mUIManager->Render();
}

void CScene::EndFrame()
{
	std::list<CSharedPtr<CSceneObject>>::iterator	iter;
	std::list<CSharedPtr<CSceneObject>>::iterator	iterEnd = mObjList.end();

	for (iter = mObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EndFrame();
	}
}
