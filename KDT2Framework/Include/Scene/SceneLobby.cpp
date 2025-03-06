#include "SceneLobby.h"
#include "UI/UserWidget/LobbyWidget.h"
#include "Scene/SceneUIManager.h"

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
    return true;
}

bool CSceneLobby::InitWidget()
{
    CLobbyWidget* Widget = mUIManager->CreateWidget<CLobbyWidget>("Lobby");

    mUIManager->AddToViewport(Widget);
    return true;
}
