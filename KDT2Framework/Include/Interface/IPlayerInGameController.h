#pragma once

#include "Interface/IPlayerGraphicController.h"

class IPlayerInGameController abstract : public IPlayerGraphicController
{
public:
	virtual bool SetMovePlayer(int playerIndex, FVector3D moveValVector) = 0;
};