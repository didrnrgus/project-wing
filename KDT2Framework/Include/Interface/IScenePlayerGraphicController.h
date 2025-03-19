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
	// 플레이어 캐릭터 종류가 아닌 현재방의 사용자캐릭을 불러오는것이다.
	virtual CSceneObject* GetPlayer(int index) = 0;

public:
	virtual bool SetChangeGraphic(int playerIndex, int graphicIndex) = 0;
};