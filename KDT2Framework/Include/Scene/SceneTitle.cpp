#include "SceneTitle.h"
#include "UI/UserWidget/TitleWidget.h"
#include "Scene/SceneUIManager.h"

CSceneTitle::CSceneTitle()
{
}

CSceneTitle::~CSceneTitle()
{
}

bool CSceneTitle::InitAsset()
{
    return true;
}

bool CSceneTitle::InitObject()
{
    return true;
}

bool CSceneTitle::InitWidget()
{
    CTitleWidget* Widget = mUIManager->CreateWidget<CTitleWidget>("Title");

    mUIManager->AddToViewport(Widget);
    return true;
}
