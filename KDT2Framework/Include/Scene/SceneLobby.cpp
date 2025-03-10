#include "SceneLobby.h"
#include "UI/UserWidget/LobbyWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"

CSceneLobby::CSceneLobby()
{
}

CSceneLobby::~CSceneLobby()
{
}

bool CSceneLobby::InitAsset()
{
    CPlayerGraphicObject* PlayerGraphic = CreateObj<CPlayerGraphicObject>("PlayerGraphic");
    players.push_back(PlayerGraphic);

    return true;
}

bool CSceneLobby::InitObject()
{
    return true;
}

bool CSceneLobby::InitWidget()
{
    CLobbyWidget* Widget = mUIManager->CreateWidget<CLobbyWidget>("Lobby");
    mUIManager->AddToViewport(Widget);
    return true;
}

CSceneObject* CSceneLobby::GetPlayer(int index)
{
    return players[index];
}

int CSceneLobby::GetGraphicCount()
{
    auto tempPlayer = dynamic_cast<CPlayerGraphicObject*>(players[0].Get());

    if (tempPlayer == nullptr)
    {
        return -1;
    }

    auto result = tempPlayer->GetGraphicCount();
    return result;
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
