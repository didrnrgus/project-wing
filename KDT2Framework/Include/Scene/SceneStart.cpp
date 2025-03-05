#include "SceneStart.h"
#include "../UI/UserWidget/StartWidget.h"
#include "Scene/SceneUIManager.h"

CSceneStart::CSceneStart()
{
}

CSceneStart::~CSceneStart()
{
}

bool CSceneStart::InitAsset()
{
    return true;
}

bool CSceneStart::InitObject()
{
    return true;
}

bool CSceneStart::InitWidget()
{
    CStartWidget* Widget = mUIManager->CreateWidget<CStartWidget>("Start");

    mUIManager->AddToViewport(Widget);

    return true;
}
