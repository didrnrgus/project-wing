#pragma once

#include "Scene/Scene.h"
#include "Interface/IScenePlayerGraphicController.h"
#include "Interface/ISceneNetworkController.h"

class CSceneLobby : public CScene, public IScenePlayerGraphicController, public ISceneNetworkController
{
	friend class CSceneManager;

private:
	CSceneLobby();
	virtual ~CSceneLobby();
	class CLobbyWidget* mLobbyWidget;

protected:
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;

	virtual void Update(float DeltaTime) override;
	
	// ISceneNetworkController을(를) 통해 상속됨
	virtual void ProcessMessage() override;
	virtual void DistributeMessage(const struct RecvMessage& msg) override;

public:
	virtual CSceneObject* GetPlayer(int index) override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;

	const class CLobbyWidget* GetLoobWidget() { return mLobbyWidget; }

	// ISceneNetworkController을(를) 통해 상속됨
	virtual void AddListener(class IObjectNetworkController* obj) override;
	virtual void RemoveListener(class IObjectNetworkController* obj) override;
};

