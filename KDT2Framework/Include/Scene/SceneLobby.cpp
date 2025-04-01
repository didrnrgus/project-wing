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
#pragma endregion

#pragma region ISceneNetworkController
void CSceneLobby::ProcessMessage()
{
	// 메시지 큐에서 메시지 뽑는 역할.
	RecvMessage msg;
	while (CNetworkManager::GetInst()->PollMessage(msg))
	{
		DistributeMessage(msg);
	}
}

void CSceneLobby::DistributeMessage(const RecvMessage& msg)
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