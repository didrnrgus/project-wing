#pragma once
#include "Object/PlayerGraphicObject.h"
#include "Interface/IObjectNetworkController.h"

class CPlayerInGameOtherObject: public CPlayerGraphicObject, public IObjectNetworkController
{
	friend class CScene;

protected:
	CPlayerInGameOtherObject();
	CPlayerInGameOtherObject(const CPlayerInGameOtherObject& Obj);
	CPlayerInGameOtherObject(CPlayerInGameOtherObject&& Obj);
	virtual ~CPlayerInGameOtherObject();

protected:
	virtual bool Init() override;

	// IObjectNetworkController��(��) ���� ��ӵ�
	void AddListener() override;
	void RemoveListener() override;
	void ProcessMessage(const RecvMessage& msg) override;



};

