#pragma once

#include "Interface/IScenePlayerGraphicController.h"

/// <summary>
/// 씬에다가 상속하는 인터페이스
/// </summary>
class IScenePlayerInGameController abstract : public IScenePlayerGraphicController
{
public:
	virtual bool SetMovePlayer(int playerIndex, FVector3D moveValVector) = 0;
};