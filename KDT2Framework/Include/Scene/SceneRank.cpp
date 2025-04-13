#include "SceneRank.h"
#include "Scene/SceneUIManager.h"
#include "Scene/SceneManager.h"
#include "UI/UserWidget/RankWidget.h"

CSceneRank::CSceneRank()
{
}

CSceneRank::~CSceneRank()
{
}

bool CSceneRank::InitAsset()
{
    return true;
}

bool CSceneRank::InitObject()
{
    return true;
}

bool CSceneRank::InitWidget()
{
    mRankWidget = mUIManager->CreateWidget<CRankWidget>("mRankWidget");
    mUIManager->AddToViewport(mRankWidget);

    return true;
}
