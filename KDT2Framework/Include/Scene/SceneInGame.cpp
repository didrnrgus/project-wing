#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/PlayerInGameObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"
#include "Etc/DataStorageManager.h"
#include "Etc/ConstValues.h"
#include "Interface/IPlayerStatController.h"
#include "Interface/IGamePlayStateController.h"
#include "Interface/IGamePlayShakeController.h"
#include "Scene/Input.h"

CSceneInGame::CSceneInGame()
{
}

CSceneInGame::~CSceneInGame()
{
}

bool CSceneInGame::Init()
{
	CScene::Init();

	GetInput()->AddBindKey("StartAndStop", VK_SPACE);
	GetInput()->AddBindFunction("StartAndStop", EInputType::Down
		, [this](float DeltaTime)
		{
			CLog::PrintLog("StartAndStop Trigger Lambda");

			if (GetGamePlayState() != EGamePlayState::Freze)
				return;

			if (GetGamePlayState() != EGamePlayState::Start)
				SetGamePlayState(EGamePlayState::Start);
			else
				SetGamePlayState(EGamePlayState::Pause);
		});

	SetGamePlayState(EGamePlayState::ReadyCount);
	mCurReadyCount = mInGameWidget->GetStartCountArrCount();
	mReadyCountTime = 0.0f;
	return true;
}

void CSceneInGame::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);

	if (mGamePlayState == EGamePlayState::ReadyCount)
	{
		mReadyCountTime += DeltaTime;
		if (mReadyCountTime >= 1.0f)
		{
			mReadyCountTime = 0.0f;
			mCurReadyCount -= 1;

			mInGameWidget->SetStartCount(mCurReadyCount);

			if (mCurReadyCount < 0)
				SetGamePlayState(EGamePlayState::Start);
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
	CPlayerInGameObject* playerInGame = CreateObj<CPlayerInGameObject>("PlayerInGame");

	// players init
	players.resize(5, nullptr); // 미리 칸 만들어놓기.

	// my player setting
	players[0] = playerInGame;
	playerInGame->SetIsMine(true);

	// 플레이어 오브젝트에 카메라 쉐이크 인터페이스 등록.
	auto cameraShake = dynamic_cast<IGamePlayShakeController*>(camera);
	if (cameraShake)
	{
		playerInGame->SetShakeCamera(cameraShake);
	}

	// 선택한 캐릭의 스탯 인터페이스 가져오기.
	auto playerInGameStat = dynamic_cast<IPlayerStatController*>(playerInGame);
	if (playerInGameStat != nullptr)
	{
		// lineGroup setting
		lineGroup->SetTargetStat(playerInGameStat);
	}

	SetChangeGraphic(0, CDataStorageManager::GetInst()->GetSelectedCharacterIndex());

	// IGamePlayStateController arr setting
	// 맵도 포함시키고.
	auto lineGroupGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(lineGroup);
	if (lineGroupGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(lineGroupGamePlayStateCtlr);

	// 이거 내가 조작하는 플레이어만 포함시키자.
	auto playerGamePlayStateCtlr = dynamic_cast<IGamePlayStateController*>(playerInGame);
	if (playerGamePlayStateCtlr)
		mArrGamePlayStateCtlr.push_back(playerGamePlayStateCtlr);

	SetGamePlayState(EGamePlayState::Ready);
	return true;
}

bool CSceneInGame::InitWidget()
{
	mInGameWidget = mUIManager->CreateWidget<CInGameWidget>(SCENE_INGAME_WIDGET);
	mUIManager->AddToViewport(mInGameWidget);
	return true;
}

#pragma region IPlayerController

CSceneObject* CSceneInGame::GetPlayer(int index)
{
	return players[index];
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

bool CSceneInGame::SetMovePlayer(int playerIndex, FVector3D moveValueVector)
{
	auto tempPlayer = dynamic_cast<CPlayerInGameObject*>(players[playerIndex].Get());

	if (tempPlayer == nullptr)
	{
		return false;
	}

	tempPlayer->SetMovePlayer(moveValueVector);
	return true;
}

void CSceneInGame::SetGamePlayState(EGamePlayState::Type type)
{
	// 씬 상태 바꾸고.
	mGamePlayState = type;

	// 다른애들한테 알려주고.  map, myPlayer
	for (auto e : mArrGamePlayStateCtlr)
	{
		e->SetGamePlayState(type);
	}
}

#pragma endregion
