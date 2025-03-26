#pragma once

#include "SceneLobby.h"
#include "UI/UserWidget/LobbyWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/CameraObject.h"
#include "Etc/NetworkManager.h"
#include "Interface/IObjectNetworkController.h"

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

void CSceneLobby::DistributeMessage(RecvMessage& msg)
{
	for (auto it : mObjNetworkController)
	{
		(it)->ProcessMessage(msg);
	}
}

void CSceneLobby::AddListener(IObjectNetworkController* obj)
{
	mObjNetworkController.push_back(obj);
}

void CSceneLobby::RemoveListener(IObjectNetworkController* obj)
{
	if (obj)
	{
		auto it = std::find_if(mObjNetworkController.begin(), mObjNetworkController.end(),
			[obj](const IObjectNetworkController* const _obj)
			{
				return _obj == obj;
			});

		if (it != mObjNetworkController.end())
		{
			mObjNetworkController.erase(it);
		}
	}
}

#pragma endregion

#pragma region ISceneNetworkController

void CSceneLobby::ProcessMessage()
{
	// 메시지 큐에서 메시지 뽑는 역할.
	RecvMessage msg;

	if (CNetworkManager::GetInst()->PollMessage(msg))
	{
		// 순서: 커넥트 -> 방장누구? -> 기존인원 리스트
		switch (msg.msgType)
		{
		case (int)ServerMessage::Type::MSG_JOIN:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_DISCONNECT:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_NEW_OWNER:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_PICK_MAP:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_PICK_ITEM:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_PICK_CHARACTER:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_READY:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_UNREADY:
		{
			break;
		}
		case (int)ServerMessage::Type::MSG_START_ACK:
		{
			break;
		}
		}
	}
}

#pragma endregion