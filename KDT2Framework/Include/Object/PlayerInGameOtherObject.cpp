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
	if (msg.senderId != GetNetID())
		return;

	switch (msg.msgType)
	{
	case (int)ServerMessage::MSG_TAKEN_DAMAGE:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		// mini HP 연결?
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_DEAD:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
	
		if (info.isDeadInGame)
		{
			// dead sign
			mDeadSign->SetRelativePos(FVector3D(0.0f, 0.0f, -0.1f));
			mDeadSign->SetEnable(info.isDeadInGame);
		}
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_DISTANCE:
	{
		// 현재 타게팅된 플레이어를 기준으로 상대적인 화면 위치를 지정해야 함.
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		float otherDist = info.distance;

		// 타겟팅 하고있는 플레이어.
		auto inGameScene = dynamic_cast<CSceneInGame*>(mScene);
		auto myPlayerObject = inGameScene->GetMyPlayerObject();
		auto myPlayerStat = dynamic_cast<IPlayerStatController*>(myPlayerObject);
		float myDist = myPlayerStat->GetPlayDistance();

		SetDebugText(std::wstring(L"distance: " + std::to_wstring(otherDist) + L"\ntargetDist: " + std::to_wstring(myDist)).c_str());

		// 타겟(나)이 앞에있다면, +가 나옴.
		float gapBaseOnTarget = myDist - otherDist;
		FVector3D worldPos = FVector3D(myPlayerObject->GetWorldPosition().x - gapBaseOnTarget * 100.0f, info.height, 0.0f);
		SetMovePlayer(worldPos);
		break;
	}
	case (int)ServerMessage::MSG_PLAYER_HEIGHT:
	{
		auto info = CMultiplayManager::GetInst()->GetPlayerInfoValueById(GetNetID());
		float otherHeight = info.height;

		FVector3D worldPos = GetWorldPosition();
		worldPos.y = otherHeight;
		SetMovePlayer(worldPos);
		break;
	}
	default:
		break;
	}
}
