#include "SceneTitle.h"
#include "UI/UserWidget/TitleWidget.h"
#include "Scene/SceneUIManager.h"
#include "Etc/NetworkManager.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneManager.h"
#include "Interface/IObjectNetworkController.h"

CSceneTitle::CSceneTitle()
{
}

CSceneTitle::~CSceneTitle()
{
}

bool CSceneTitle::InitAsset()
{
	return true;
}

bool CSceneTitle::InitObject()
{
	return true;
}

bool CSceneTitle::InitWidget()
{
	mTitleWidget = mUIManager->CreateWidget<CTitleWidget>("mTitleWidget");
	mUIManager->AddToViewport(mTitleWidget);

	return true;
}

void CSceneTitle::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);

	ProcessMessage();
}

void CSceneTitle::ProcessMessage()
{
	RecvMessage msg;
	if (CNetworkManager::GetInst()->PollMessage(msg))
	{
		DistributeMessage(msg);

		switch (msg.msgType)
		{
		case (int)ServerMessage::Type::MSG_ROOM_FULL_INFO:
		{
			// 멀티모드에서는 커넥트 성공했을때 씬 넘긴다.
			CSceneManager::GetInst()->CreateLoadScene<CSceneLobby>();
			break;
		}
		case (int)ServerMessage::Type::MSG_CONNECTED_REJECT:
		{
			// 이유 알려주는 UI 필요.
			CNetworkManager::GetInst()->SetIsMultiplay(false);
			mTitleWidget->ShowLoading(false);
			break;
		}
		default:
			break;
		}
	}

}

void CSceneTitle::DistributeMessage(const RecvMessage& msg)
{
	for (auto it : mObjNetworkController)
	{
		(it)->ProcessMessage(msg);
	}
}

void CSceneTitle::AddListener(IObjectNetworkController* obj)
{
	mObjNetworkController.push_back(obj);
}

void CSceneTitle::RemoveListener(IObjectNetworkController* obj)
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


