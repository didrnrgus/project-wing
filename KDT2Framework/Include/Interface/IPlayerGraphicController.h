#pragma once

#include "GameInfo.h"

class IPlayerGraphicController abstract
{
protected:
	std::vector<CSharedPtr<class CSceneObject>> players;

public:
	virtual CSceneObject* GetPlayer(int index) = 0;

public:
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) = 0;
};