#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/PlayerInGameObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"
#include <Etc/DataStorageManager.h>

CSceneInGame::CSceneInGame()
{
}

CSceneInGame::~CSceneInGame()
{
}

bool CSceneInGame::InitAsset()
{
    return true;
}

bool CSceneInGame::InitObject()
{
    CCameraObject* camera = CreateObj<CCameraObject>("Camera");
    CPlayerInGameObject* playerInGame = CreateObj<CPlayerInGameObject>("PlayerInGame");
    players.push_back(playerInGame);
    SetChangeGraphic(0, CDataStorageManager::GetInst()->GetSelectedCharacterIndex());

    CLineGroupObject* lineGroup = CreateObj<CLineGroupObject>("LineGroupObject");

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

#pragma endregion
