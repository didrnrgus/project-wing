#pragma once

#include "ConstantBufferData.h"

class CTileMapCBuffer :
    public CConstantBufferData
{
public:
	CTileMapCBuffer();
	CTileMapCBuffer(const CTileMapCBuffer& Data);
	CTileMapCBuffer(CTileMapCBuffer&& Data);
	virtual ~CTileMapCBuffer();

private:
	FTileMapCBufferInfo	mData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTileMapCBuffer* Clone();

public:
	void SetUV(const FVector2D& LT, const FVector2D& RB)
	{
		mData.LTUV = LT;
		mData.RBUV = RB;
	}
};

