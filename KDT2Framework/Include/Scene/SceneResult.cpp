#pragma once
#include "SceneResult.h"
#include "Scene/SceneUIManager.h"
#include "UI/UserWidget/ResultWidget.h"

CSceneResult::CSceneResult()
{
}

CSceneResult::~CSceneResult()
{
}

bool CSceneResult::Init()
{
	CScene::Init();

	return true;
}

bool CSceneResult::InitAsset()
{

	return true;
}

bool CSceneResult::InitObject()
{

	return true;
}

bool CSceneResult::InitWidget()
{
	mResultWidget = mUIManager->CreateWidget<CResultWidget>("mResultWidget");
	mUIManager->AddToViewport(mResultWidget);
	return true;
}
