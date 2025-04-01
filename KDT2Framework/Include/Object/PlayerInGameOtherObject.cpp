#pragma once
#include "PlayerInGameOtherObject.h"
#include "Etc/MultiplayManager.h"
#include "Interface/ISceneNetworkController.h"
#include "Interface/IPlayerStatController.h"
#include "Component/SpriteComponent.h"
#include "Scene/SceneInGame.h"
#include "Object/PlayerInGameObject.h"


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
	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_TAKEN_DAMAGE:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		if (info.id == mNetID)
		{
			// mini HP ����?

		}
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_DEAD:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		if (info.id == mNetID)
		{
			// dead sign
			mDeadSign->SetRelativePos(FVector3D(0.0f, 0.0f, -0.1f));
			mDeadSign->SetEnable(info.isDeadInGame);
		}
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_DISTANCE:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		if (info.id == mNetID)
		{
			// ���� Ÿ���õ� �÷��̾ �������� ������� ȭ�� ��ġ�� �����ؾ� ��.
			float otherDist = info.distance;
			auto inGameScene = dynamic_cast<CSceneInGame*>(mScene);
			auto myPlayerObject = inGameScene->GetMyPlayerObject();
			auto myPlayerStat = dynamic_cast<IPlayerStatController*>(myPlayerObject);
			float myDist = myPlayerStat->GetPlayDistance();
			float gapBaseOnTarget = myDist - otherDist; // Ÿ��(��)�� �տ��ִٸ�, +�� ����.
			FVector3D worldPos = FVector3D(myPlayerObject->GetWorldPosition().x - gapBaseOnTarget, info.height, 0.0f);
			SetMovePlayer(worldPos);
		}
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_HEIGHT:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		if (info.id == mNetID)
		{
			float otherHeight = info.height;
			FVector3D worldPos = GetWorldPosition();
			worldPos.y = otherHeight;
			SetMovePlayer(worldPos);
		}
		break;
	}
	default:
		break;
	}
}
