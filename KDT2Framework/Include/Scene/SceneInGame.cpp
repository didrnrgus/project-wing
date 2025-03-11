#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"

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
    return nullptr;
}

int CSceneInGame::GetGraphicCount()
{
    return 0;
}

bool CSceneInGame::SetChangeGraphic(int playerIndex, int graphicIndex)
{
    return false;
}

#pragma endregion
