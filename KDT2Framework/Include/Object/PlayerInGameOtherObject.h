#pragma once
#include "Object/PlayerGraphicObject.h"
#include "Interface/IObjectNetworkController.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"

class CPlayerInGameOtherObject: public CPlayerGraphicObject, public IObjectNetworkController, public IPlayerStatController, public IGamePlayStateController
{
	friend class CScene;

protected:
	CPlayerInGameOtherObject();
	CPlayerInGameOtherObject(const CPlayerInGameOtherObject& Obj);
	CPlayerInGameOtherObject(CPlayerInGameOtherObject&& Obj);
	virtual ~CPlayerInGameOtherObject();

protected:
	virtual bool Init() override;

	// IObjectNetworkController을(를) 통해 상속됨
	void AddListener() override;
	void RemoveListener() override;
	void ProcessMessage(const RecvMessage& msg) override;



};

