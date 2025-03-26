#pragma once
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Interface/ISceneNetworkController.h"

class IObjectNetworkController abstract
{
protected:
	/*
		auto curScene = CSceneManager::GetInst()->GetCurrentScene();
		if (curScene == nullptr)
			return;

		auto netController = dynamic_cast<ISceneNetworkController*>(curScene);
		netController->AddListener(this);
		// netController->RemoveListener(this);
	*/
	virtual void AddListener() = 0;
	virtual void RemoveListener() = 0;
	/*void AddListener()
	{
		auto curScene = CSceneManager::GetInst()->GetCurrentScene();
		if (curScene == nullptr)
			return;

		auto sceneNetController = dynamic_cast<ISceneNetworkController*>(curScene);
		if (sceneNetController == nullptr)
			return;

		sceneNetController->AddListener(this);
	}

	void RemoveListener()
	{
		auto curScene = CSceneManager::GetInst()->GetCurrentScene();
		if (curScene == nullptr)
			return;

		auto sceneNetController = dynamic_cast<ISceneNetworkController*>(curScene);
		if (sceneNetController == nullptr)
			return;
		sceneNetController->RemoveListener(this);
	}*/

public:
	virtual void ProcessMessage(struct RecvMessage& msg) = 0;
};