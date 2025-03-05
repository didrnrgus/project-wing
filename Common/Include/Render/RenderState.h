#pragma once

#include "../GameInfo.h"

class CRenderState
{
	friend class CRenderStateManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	ID3D11DeviceChild* mState = nullptr;
	ID3D11DeviceChild* mPrevState = nullptr;
	ERenderStateType::Type	mStateType;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

