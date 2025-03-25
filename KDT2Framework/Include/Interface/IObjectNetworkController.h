#pragma once

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

public:
	virtual void ProcessMessage(struct RecvMessage& msg) = 0;
};