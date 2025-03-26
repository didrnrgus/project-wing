#pragma once
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Interface/ISceneNetworkController.h"

class IObjectNetworkController abstract
{
protected:
	virtual void AddListener() = 0;
	/*void AddListener()
	{
		auto curScene = CSceneManager::GetInst()->GetCurrentScene();
		if (curScene == nullptr)
			return;

		auto sceneNetController = dynamic_cast<ISceneNetworkController*>(curScene);
		if (sceneNetController == nullptr)
			return;

		sceneNetController->AddListener(this);
	}	*/

	virtual void RemoveListener() = 0;
	/*void RemoveListener()
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
	virtual void ProcessMessage(const struct RecvMessage& msg) = 0;
	/*
		void ProcessMessage(const RecvMessage& msg)
		{
			switch (msg.msgType)
			{
			case (int)ServerMessage::Type::MSG_..:
			default:
				break;
			}
		}
	*/
};