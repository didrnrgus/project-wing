#pragma once

#include "SceneLobby.h"
#include "UI/UserWidget/LobbyWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/CameraObject.h"
#include "Etc/MultiplayManager.h"

CSceneLobby::CSceneLobby()
{
}

CSceneLobby::~CSceneLobby()
{
}

bool CSceneLobby::InitAsset()
{
	return true;
}

bool CSceneLobby::InitObject()
{
	CCameraObject* Camera = CreateObj<CCameraObject>("Camera");
	CPlayerGraphicObject* PlayerGraphic = CreateObj<CPlayerGraphicObject>("PlayerGraphic");
	players.push_back(PlayerGraphic);

	return true;
}

bool CSceneLobby::InitWidget()
{
	mLobbyWidget = mUIManager->CreateWidget<CLobbyWidget>("mLobbyWidget");
	mUIManager->AddToViewport(mLobbyWidget);
	return true;
}

void CSceneLobby::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
	ProcessMessage();
}

#pragma region IScenePlayerGraphicController

CSceneObject* CSceneLobby::GetPlayer(int index)
{
	return players[index];
}

bool CSceneLobby::SetChangeGraphic(int playerIndex, int graphicIndex)
{
	auto tempPlayer = dynamic_cast<CPlayerGraphicObject*>(players[playerIndex].Get());

	if (tempPlayer == nullptr)
	{
		return false;
	}

	auto result = tempPlayer->SetChangeGraphic(graphicIndex);
	return result;
}

#pragma endregion

#pragma region ISceneNetworkController

void CSceneLobby::ProcessMessage()
{
	// 메시지 큐에서 메시지 뽑는 역할.
	RecvMessage msg;

	if (CMultiplayManager::GetInst()->PollMessage(msg))
	{
		switch (msg.msgType)
		{
		case (int)ServerMessage::Type::MSG_CONNECTED:
		{
			int id;
			memcpy(&id, msg.body.data(), sizeof(int));
			CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] Connected. My ID: " + std::to_string(id));
			break;
		}
		case (int)ServerMessage::Type::MSG_NEW_OWNER:
		{
			int id;
			memcpy(&id, msg.body.data(), sizeof(int));
			CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] New Room Owner: " + std::to_string(id));
			break;
		}
		case (int)ServerMessage::Type::MSG_PICK_MAP:
		{
			if (msg.body.size() >= sizeof(int))
			{
				int mapId;
				memcpy(&mapId, msg.body.data(), sizeof(int));
				CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] Map changed to " + std::to_string(mapId));
			}
			break;
		}
		case (int)ServerMessage::Type::MSG_PICK_ITEM:
		{
			if (msg.body.size() >= sizeof(int) * 2)
			{
				int slot, itemId;
				memcpy(&slot, msg.body.data(), sizeof(int));
				memcpy(&itemId, msg.body.data() + sizeof(int), sizeof(int));
				CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] Player " + std::to_string(msg.senderId) + " picked item " + std::to_string(itemId) + " in slot " + std::to_string(slot));
			}
			break;
		}

		case (int)ServerMessage::Type::MSG_PICK_CHARACTER:
		{
			if (msg.body.size() >= sizeof(int))
			{
				int characterId;
				memcpy(&characterId, msg.body.data(), sizeof(int));
				CLog::PrintLog("[Game " + std::to_string(msg.msgType) + "] Player " + std::to_string(msg.senderId) + " picked character " + std::to_string(characterId));
			}
			break;
		}
		
		default:
			if (msg.msgType != (int)ServerMessage::Type::MSG_HEARTBEAT_ACK)
				CLog::PrintLog("[MSG " + std::to_string(msg.msgType) + "] From " + std::to_string(msg.senderId));
			break;
		}
	}
}

void CSceneLobby::ProcessMessage(RecvMessage& msg)
{
	// 메시지 받는역할.

}

#pragma endregion