#include "SceneLobby.h"
#include "UI/UserWidget/LobbyWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include <Object/CameraObject.h>

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
    CLobbyWidget* Widget = mUIManager->CreateWidget<CLobbyWidget>("Lobby");
    mUIManager->AddToViewport(Widget);
    return true;
}

#pragma region IPlayerController

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
