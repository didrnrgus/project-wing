#pragma once
#include "Scene/Scene.h"
#include "Interface/ISceneNetworkController.h"

class CSceneTitle : public CScene, public ISceneNetworkController
{
	friend class CSceneManager;

private:
	class CTitleWidget* mTitleWidget;

private:
	CSceneTitle();
	virtual ~CSceneTitle();

protected:
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;
	virtual void Update(float DeltaTime) override;

	// ISceneNetworkController을(를) 통해 상속됨
	virtual void ProcessMessage() override;
	virtual void DistributeMessage(const struct RecvMessage& msg) override;

public:
	// ISceneNetworkController을(를) 통해 상속됨
	virtual void AddListener(class IObjectNetworkController* obj) override;
	virtual void RemoveListener(class IObjectNetworkController* obj) override;
};

