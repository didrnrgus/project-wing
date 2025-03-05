#pragma once

#include "ConstantBufferData.h"

class CAnimation2DCBuffer :
    public CConstantBufferData
{
public:
	CAnimation2DCBuffer();
	CAnimation2DCBuffer(const CAnimation2DCBuffer& Data);
	CAnimation2DCBuffer(CAnimation2DCBuffer&& Data);
	virtual ~CAnimation2DCBuffer();

private:
	FAnimation2DCBufferInfo	mData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CAnimation2DCBuffer* Clone();

public:
	void SetUV(float LTX, float LTY, float RBX, float RBY)
	{
		mData.LTUV.x = LTX;
		mData.LTUV.y = LTY;
		mData.RBUV.x = RBX;
		mData.RBUV.y = RBY;
	}

	void SetAnimation2DEnable(bool Enable)
	{
		mData.AnimEnable = Enable ? 1 : 0;
	}
};

