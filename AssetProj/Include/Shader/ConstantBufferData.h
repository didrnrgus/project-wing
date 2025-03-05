#pragma once

#include "ConstantBuffer.h"

class CConstantBufferData abstract
{
public:
	CConstantBufferData();
	CConstantBufferData(const CConstantBufferData& Data);
	CConstantBufferData(CConstantBufferData&& Data);
	virtual ~CConstantBufferData();

protected:
	CSharedPtr<CConstantBuffer>	mBuffer;

protected:
	void SetConstantBuffer(const std::string& Name);

public:
	virtual bool Init() = 0;
	virtual void UpdateBuffer() = 0;
	virtual CConstantBufferData* Clone() = 0;
};

