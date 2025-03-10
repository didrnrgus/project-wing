#pragma once
#include "Scene\Scene.h"
#include "Interface/IPlayerController.h"

class CSceneLobby : public CScene, public IPlayerController
{
	friend class CSceneManager;

private:
	CSceneLobby();
	virtual ~CSceneLobby();

protected:
	virtual bool InitAsset();
	virtual bool InitObject();
	virtual bool InitWidget();

public:
	virtual CSceneObject* GetPlayer(int index) override;
	/// <summary>
	/// 플레이어 인덱스 0을 본다 -> 플레이어가 하나라도 있어야 함.
	/// </summary>
	/// <returns></returns>
	virtual int GetGraphicCount() override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
};

