#pragma once
#include "Scene\Scene.h"
#include "Interface/IScenePlayerInGameController.h"
#include "Interface/IGamePlayStateController.h"

class CSceneInGame : public CScene, public IScenePlayerInGameController
{
	friend class CSceneManager;

private:
	CSceneInGame();
	virtual ~CSceneInGame();

private:
	std::vector<IGamePlayStateController*> mArrGamePlayCtlr;
	EGamePlayState::Type mGamePlayState = EGamePlayState::Ready;

protected:
	virtual bool Init() override;
	virtual bool InitAsset() override;
	virtual bool InitObject() override;
	virtual bool InitWidget() override;
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) override;
	virtual bool SetMovePlayer(int playerIndex, FVector3D moveValueVector) override;
	
	void SetGamePlayState(EGamePlayState::Type type);
	EGamePlayState::Type GetGamePlayState() { return mGamePlayState; }

public:
	// 플레이어 캐릭터 종류가 아닌 현재방의 사용자캐릭을 불러오는것이다.
	virtual CSceneObject* GetPlayer(int index) override;
};


