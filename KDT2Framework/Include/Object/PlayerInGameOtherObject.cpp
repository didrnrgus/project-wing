#pragma once
#include "PlayerInGameOtherObject.h"

CPlayerInGameOtherObject::CPlayerInGameOtherObject()
{
}

CPlayerInGameOtherObject::CPlayerInGameOtherObject(const CPlayerInGameOtherObject& Obj)
{
}

CPlayerInGameOtherObject::CPlayerInGameOtherObject(CPlayerInGameOtherObject&& Obj)
{
}

CPlayerInGameOtherObject::~CPlayerInGameOtherObject()
{
	RemoveListener();
}

bool CPlayerInGameOtherObject::Init()
{
	CPlayerGraphicObject::Init();
	AddListener();
	return true;
}

void CPlayerInGameOtherObject::AddListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->AddListener(this);
}

void CPlayerInGameOtherObject::RemoveListener()
{
	auto sceneNetController = dynamic_cast<ISceneNetworkController*>(mScene);
	if (sceneNetController == nullptr)
		return;
	sceneNetController->RemoveListener(this);
}

void CPlayerInGameOtherObject::ProcessMessage(const RecvMessage& msg)
{
	/*switch (msg.msgType)
	{
		case (int)ServerMessage::MSG_MOVE_UP
	default:
		break;
	}*/
}
