#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/PlayerInGameObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"
#include "Etc/DataStorageManager.h"
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

    #ifdef _DEBUG
	GetInput()->AddBindKey("StartAndStop", VK_SPACE);
	GetInput()->AddBindFunction("StartAndStop", EInputType::Down
        , [this](float DeltaTime)
        {
            CLog::PrintLog("StartAndStop Trigger Lambda");
            if(GetGamePlayState() != EGamePlayState::Start)
                SetGamePlayState(EGamePlayState::Start);
            else
                SetGamePlayState(EGamePlayState::Pause);
        });
    #endif // _DEBUG

    return true;
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
    
    // 선택한 캐릭의 초기 스텟 등록.
    auto initializedStatData = CDataStorageManager::GetInst()->GetSelectedCharacterState();
    auto playerInGameStat = dynamic_cast<IPlayerStatController*>(playerInGame);
    if (playerInGameStat != nullptr)
    {
        playerInGameStat->Init(initializedStatData);
    }

    SetChangeGraphic(0, CDataStorageManager::GetInst()->GetSelectedCharacterIndex());

    // lineGroup setting
    lineGroup->SetTargetStat(playerInGameStat);

    // IGamePlayStateController arr setting
    auto lineGroupGamePlayCtlr = dynamic_cast<IGamePlayStateController*>(lineGroup);
    if (lineGroupGamePlayCtlr)
        mArrGamePlayCtlr.push_back(lineGroupGamePlayCtlr);

    auto playerGamePlayCtlr = dynamic_cast<IGamePlayStateController*>(playerInGame);
    if (playerGamePlayCtlr)
        mArrGamePlayCtlr.push_back(playerGamePlayCtlr);

    SetGamePlayState(EGamePlayState::Ready);
    return true;
}

bool CSceneInGame::InitWidget()
{
    CInGameWidget* Widget = mUIManager->CreateWidget<CInGameWidget>("InGame");
    mUIManager->AddToViewport(Widget);
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
    mGamePlayState = type;

    for (auto e : mArrGamePlayCtlr)
    {
        e->SetGamePlayState(type);
    }
}

#pragma endregion
