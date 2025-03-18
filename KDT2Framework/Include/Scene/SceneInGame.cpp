#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/PlayerInGameObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"
#include "Etc/DataStorageManager.h"
#include "Interface/IPlayerStatController.h"
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
    players.resize(5, nullptr); // 미리 칸 만들어놓기.

    // myPlayer setting
    players[0] = playerInGame;
    playerInGame->SetIsMine(true);
    auto firstStatData = CDataStorageManager::GetInst()->GetSelectedCharacterState();
    auto playerInGameStat = dynamic_cast<IPlayerStatController*>(playerInGame);
    if (playerInGameStat != nullptr)
    {
        playerInGameStat->Init(firstStatData);
    }
    lineGroup->SetTargetStat(playerInGameStat);

    SetChangeGraphic(0, CDataStorageManager::GetInst()->GetSelectedCharacterIndex());

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

    auto result = tempPlayer->SetChangeGraphic(graphicIndex);
    return result;
}

bool CSceneInGame::SetMovePlayer(int playerIndex, FVector3D moveValVector)
{
    auto tempPlayer = dynamic_cast<CPlayerInGameObject*>(players[playerIndex].Get());

    if (tempPlayer == nullptr)
    {
        return false;
    }

    auto result = tempPlayer->SetMovePlayer(moveValVector);
    return result;
}

#pragma endregion
