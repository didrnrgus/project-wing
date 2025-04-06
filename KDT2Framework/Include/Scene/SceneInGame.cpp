#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneLobby.h"
#include "Scene/SceneTitle.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/PlayerInGameObject.h"
#include "Object/PlayerInGameOtherObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"
#include "Object/ObstacleGroupObject.h"
#include "Etc/DataStorageManager.h"
#include "Etc/ConstValues.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IGamePlayShakeController.h"
#include "Scene/Input.h"
#include "Etc/NetworkManager.h"
#include "Etc/MultiplayManager.h"
#include "Etc/TaskManager.h"

CSceneInGame::CSceneInGame()
{
}

CSceneInGame::~CSceneInGame()
{
}

bool CSceneInGame::Init()
{
	CScene::Init();

	//GetInput()->AddBindKey("StartAndStop", 'Z');
	//GetInput()->AddBindFunction("StartAndStop", EInputType::Down
	//	, [this](float DeltaTime)
	//	{
	//		CLog::PrintLog("StartAndStop Trigger Lambda");

	//		if (GetGamePlayState() != EGamePlayState::Freze)
	//			return;

	//		if (GetGamePlayState() != EGamePlayState::Start)
	//			SetGamePlayState(EGamePlayState::Start);
	//		else
	//			SetGamePlayState(EGamePlayState::Pause);
	//	});

	SetGamePlayState(EGamePlayState::ReadyCount);
	mCurReadyCount = mInGameWidget->GetStartCountArrCount();
	mReadyCountTime = 0.0f;

	return true;
}

void CSceneInGame::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
	ProcessMessage();

	if (mGamePlayState == EGamePlayState::ReadyCount)
	{
		mReadyCountTime += DeltaTime;
		if (mReadyCountTime >= 1.0f)
		{
			mReadyCountTime = 0.0f;
			mCurReadyCount -= 1;

			mInGameWidget->SetStartCount(mCurReadyCount);

			if (mCurReadyCount < 0)
			{
				if (!CNetworkManager::GetInst()->IsMultiplay())
					SetGamePlayState(EGamePlayState::Start);
			}
		}
	}
}

bool CSceneInGame::InitAsset()
{
	return true;
}

bool CSceneInGame::InitObject()
{
	CCameraObject* camera = CreateObj<CCameraObject>("Camera");
	CLineGroupObject* lineGroup = CreateObj<CLineGroupObject>("LineGroupObject");
	CObstacleGroupObject* obstacle = CreateObj<CObstacleGroupObject>("Obstacle");

	// players init
	players.resize(5, nullptr); // 미리 칸 만들어놓기.

	if (CNetworkManager::GetInst()->IsMultiplay())
	{
		auto myPlayerInfo = CMultiplayManager::GetInst()->GetPlayerInfoFromMyId();
		int playerCount = CMultiplayManager::GetInst()->GetPlayerCount();

		for (int i = 0; i < playerCount; i++)
		{
			CPlayerGraphicObject* _playerObject = nullptr;
			auto _netPlayerInfo = CMultiplayManager::GetInst()->GetPlayerInfoByIndex(i);
			int _id = _netPlayerInfo.id;
			bool _isMine = myPlayerInfo.id == _id;

			if (_isMine)
			{
				mMyPlayerObject = CreateObj<CPlayerInGameObject>("[IsMine]PlayerInGameObject" + std::to_string(_id));
				_playerObject = mMyPlayerObject;

				auto playerGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(mMyPlayerObject);
				if (playerGamePlayStateCtlr)
					mArrGamePlayStateCtlr.push_back(playerGamePlayStateCtlr);
			}
			else
			{
				auto _playerOtherObject = CreateObj<CPlayerInGameOtherObject>("PlayerInGameObject" + std::to_string(_id));
				_playerObject = _playerOtherObject;

				auto playerGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(_playerOtherObject);
				if (playerGamePlayStateCtlr)
					mArrGamePlayStateCtlr.push_back(playerGamePlayStateCtlr);
			}

			_playerObject->SetIsMine(_isMine);
			players[i] = _playerObject;

			auto _tempNetworkController = dynamic_cast<IObjectNetworkController*>(_playerObject);
			if (_tempNetworkController != nullptr)
				_tempNetworkController->SetNetID(_id);

			_playerObject->SetIndexInScene(i);

			// 캐릭터 왼관 세팅
			SetChangeGraphic(i, _netPlayerInfo.characterType);
		}
	}
	else
	{
		mMyPlayerObject = CreateObj<CPlayerInGameObject>("PlayerInGame");

		// my player setting
		players[0] = mMyPlayerObject;
		mMyPlayerObject->SetIsMine(true);

		// 캐릭터 왼관 세팅
		SetChangeGraphic(0, CDataStorageManager::GetInst()->GetSelectedCharacterIndex());
	
		// 이거 내가 조작하는 플레이어만 포함시키자.
		auto playerGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(mMyPlayerObject);
		if (playerGamePlayStateCtlr)
			mArrGamePlayStateCtlr.push_back(playerGamePlayStateCtlr);
	}

	if (mMyPlayerObject == nullptr)
		return false;

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

	// IGamePlayStateController arr setting
	auto lineGroupGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(lineGroup);
	if (lineGroupGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(lineGroupGamePlayStateCtlr);

	auto obstacleGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(obstacle);
	if (obstacleGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(obstacleGamePlayStateCtlr);


	SetGamePlayState(EGamePlayState::Ready);
	return true;
}

bool CSceneInGame::InitWidget()
{
	mInGameWidget = mUIManager->CreateWidget<CInGameWidget>(SCENE_INGAME_WIDGET);
	mUIManager->AddToViewport(mInGameWidget);

	auto _gamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(mInGameWidget);
	if (_gamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(_gamePlayStateCtlr);
	return true;
}

#pragma region IPlayerController

CSceneObject* CSceneInGame::GetPlayer(int index)
{
	return players[index];
}

bool CSceneInGame::GetOtherPlayerStatByNetId(int _netId, IPlayerStatController*& _playerStatOut)
{
	for (auto _player : players)
	{
		if (!_player)
			continue;

		auto _playerObject = dynamic_cast<CPlayerInGameOtherObject*>(_player.Get());

		if (!_playerObject)
			continue;

		auto _netObject = dynamic_cast<IObjectNetworkController*>(_playerObject);

		if (!_netObject)
			continue;

		if (_netId != _netObject->GetNetID())
			continue;

		auto _playerStat = dynamic_cast<IPlayerStatController*>(_playerObject);

		if (!_playerStat)
			continue;

		_playerStatOut = _playerStat;
		return true;
	}

	_playerStatOut = nullptr;
	return false;
}

bool CSceneInGame::SetChangeGraphic(int playerIndex, int graphicIndex)
{
	auto tempPlayer = dynamic_cast<CPlayerGraphicObject*>(players[playerIndex].Get());

	if (tempPlayer == nullptr)
	{
		return false;
	}

	tempPlayer->SetChangeGraphic(graphicIndex);
	return true;
}

void CSceneInGame::SetGamePlayState(EGamePlayState::Type type)
{
	// 씬 상태 바꾸고.
	mGamePlayState = type;

	// 다른애들한테 알려주고.  map, myPlayer
	for (auto& e : mArrGamePlayStateCtlr)
	{
		e->SetGamePlayState(type);
	}
}

void CSceneInGame::ProcessMessage()
{
	// 메시지 큐에서 메시지 뽑는 역할.
	RecvMessage msg;

	while (CNetworkManager::GetInst()->PollMessage(msg))
	{
		DistributeMessage(msg);

		switch (msg.msgType)
		{
		case (int)ServerMessage::MSG_COUNTDOWN_FINISHED:
		{
			mInGameWidget->SetStartCount(-1);
			SetGamePlayState(EGamePlayState::Start);
			break;
		}
		case (int)ServerMessage::MSG_PLAYER_DEAD:
		{
			// 내꺼 가져옴. -> 조종하고 있는 플레이어.
			auto myPlayerNetInfo = CMultiplayManager::GetInst()->GetPlayerInfoFromMyId();
			if (myPlayerNetInfo.isDeadInGame)
				SetGamePlayState(EGamePlayState::Dead);
			break;
		}
		case (int)ServerMessage::MSG_GAME_OVER:
		{
			CMultiplayManager::GetInst()->ResetPlayerAfterInGame();

			// 쓰레드 시작.
			mTaskID = CTaskManager::GetInst()->AddTask(std::move(std::thread(
				[this]()
				{
					CLog::PrintLog("CPlayerInGameObject::ProcessMessage MSG_GAME_OVER");
					CLog::PrintLog("std::this_thread::sleep_for(std::chrono::milliseconds(3000));");
					std::this_thread::sleep_for(std::chrono::milliseconds(3000));

					GotoLobby();
				})));
			break;
		}
		default:
			break;
		}
	}
}

void CSceneInGame::DistributeMessage(const RecvMessage& msg)
{
	for (auto& it : mObjNetworkController)
	{
		it->ProcessMessage(msg);
	}
}

void CSceneInGame::AddListener(IObjectNetworkController* obj)
{
	mObjNetworkController.push_back(obj);
}

void CSceneInGame::RemoveListener(IObjectNetworkController* obj)
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
