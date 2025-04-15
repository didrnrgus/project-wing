#include "SceneTitle.h"
#include "UI/UserWidget/TitleWidget.h"
#include "Etc/NetworkManager.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "Interface/IObjectNetworkController.h"
#include "Interface/IGamePlayStateController.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/PlayerTitleObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"
#include "Object/ObstacleGroupObject.h"


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
	// 데이터 로드가 이미 되어있다면 실행. -> 아니면 위젯에서 로드끝나고 자동실행.
	if (CDataStorageManager::GetInst()->GetIsLoadedData())
		InitObjectAfterDataLoad();

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
	while (CNetworkManager::GetInst()->PollMessage(msg))
		DistributeMessage(msg);

}

void CSceneTitle::DistributeMessage(const RecvMessage& msg)
{
	for (auto& it : mObjNetworkController)
		it->ProcessMessage(msg);
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
			mObjNetworkController.erase(it);
	}
}

void CSceneTitle::SetGamePlayState(EGamePlayState::Type type)
{
	// 씬 상태 바꾸고.
	mGamePlayState = type;

	// 다른애들한테 알려주고.  map, myPlayer
	for (auto& e : mArrGamePlayStateCtlr)
		e->SetGamePlayState(type);

}

CSceneObject* CSceneTitle::GetPlayer(int index)
{
	return players[index];
}

bool CSceneTitle::SetChangeGraphic(int playerIndex, int graphicIndex)
{
	auto tempPlayer = dynamic_cast<CPlayerGraphicObject*>(players[playerIndex].Get());

	if (tempPlayer == nullptr)
		return false;

	tempPlayer->SetChangeGraphic(graphicIndex);
	return true;
}

void CSceneTitle::InitObjectAfterDataLoad()
{
	CCameraObject* camera = CreateObj<CCameraObject>("CCameraObject");
	CLineGroupObject* lineGroup = CreateObj<CLineGroupObject>("CLineGroupObject");
	CObstacleGroupObject* obstacle = CreateObj<CObstacleGroupObject>("CObstacleGroupObject");

	players.resize(1);
	mMyPlayerObject = CreateObj<CPlayerTitleObject>("CPlayerTitleObject");
	mMyPlayerObject->SetIsMine(true);
	players[0] = mMyPlayerObject;

	SetChangeGraphic(0, CDataStorageManager::GetInst()->GetSelectedCharacterIndex());

	// 플레이어 오브젝트에 카메라 쉐이크 인터페이스 등록.
	auto cameraShake = dynamic_cast<IGamePlayShakeController*>(camera);
	if (cameraShake)
	{
		mMyPlayerObject->SetShakeCamera(cameraShake);
	}

	// 선택한 캐릭의 스탯 인터페이스 가져오기.
	auto _playerInGameStat = dynamic_cast<IPlayerStatController*>(mMyPlayerObject);
	if (_playerInGameStat != nullptr)
	{
		// lineGroup setting
		lineGroup->SetTargetStat(_playerInGameStat);
		obstacle->SetTargetStat(_playerInGameStat);
	}

	// 이거 내가 조작하는 플레이어만 포함시키자.
	auto playerGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(mMyPlayerObject);
	if (playerGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(playerGamePlayStateCtlr);

	// IGamePlayStateController arr setting
	auto lineGroupGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(lineGroup);
	if (lineGroupGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(lineGroupGamePlayStateCtlr);

	auto obstacleGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(obstacle);
	if (obstacleGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(obstacleGamePlayStateCtlr);

	lineGroup->SetIsTitle(true);
	SetGamePlayState(EGamePlayState::Start);
}


