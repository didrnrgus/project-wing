#include "SceneTitle.h"
#include "UI/UserWidget/TitleWidget.h"
#include "Scene/SceneUIManager.h"
#include "Etc/NetworkManager.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneManager.h"

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
		switch (msg.msgType)
		{
		case (int)ServerMessage::Type::MSG_CONNECTED:
		{
			int id;
			memcpy(&id, msg.body.data(), sizeof(int));
			CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] MSG_CONNECTED MyID: " + std::to_string(id));

			// 멀티모드에서는 커넥트 성공했을때 씬 넘긴다.
			CSceneManager::GetInst()->CreateLoadScene<CSceneLobby>();
			break;
		}
		case (int)ServerMessage::Type::MSG_CONNECTED_REJECT:
		{
			std::string reason(msg.body.begin(), msg.body.end());
			CLog::PrintLog("[System " + std::to_string(msg.msgType) + "] MSG_CONNECTED_REJECT Reason: " + reason);

			mTitleWidget->ShowLoading(false);
			break;
		}
		default:
			break;
		}
	}

}


