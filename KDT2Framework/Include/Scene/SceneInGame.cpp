#include "SceneInGame.h"
#include "UI/UserWidget/InGameWidget.h"
#include "Scene/SceneUIManager.h"
#include "Object/PlayerGraphicObject.h"
#include "Object/LineGroupObject.h"
#include "Object/CameraObject.h"

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
    CCameraObject* Camera = CreateObj<CCameraObject>("Camera");
    CPlayerGraphicObject* playerGraphic = CreateObj<CPlayerGraphicObject>("PlayerGraphic");
    CLineGroupObject* lineGroup = CreateObj<CLineGroupObject>("LineGroupObject");

    //lineGroup->InitLines();

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

bool CSceneInGame::SetChangeGraphic(int playerIndex, int graphicIndex)
{
    return false;
}

#pragma endregion
