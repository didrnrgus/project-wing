#pragma once

#include "GameInfo.h"

/// <summary>
/// 씬에다가 상속하는 인터페이스
/// </summary>
class IScenePlayerGraphicController abstract
{
protected:
	std::vector<CSharedPtr<class CSceneObject>> players;

public:
	virtual CSceneObject* GetPlayer(int index) = 0;

public:
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) = 0;
};