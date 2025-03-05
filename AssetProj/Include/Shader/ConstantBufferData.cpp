#include "ConstantBufferData.h"
#include "ShaderManager.h"

CConstantBufferData::CConstantBufferData()
{
}

CConstantBufferData::CConstantBufferData(
	const CConstantBufferData& Data)
{
	mBuffer = Data.mBuffer;
}

CConstantBufferData::CConstantBufferData(
	CConstantBufferData&& Data)
{
	mBuffer = Data.mBuffer;
}

CConstantBufferData::~CConstantBufferData()
{
}

void CConstantBufferData::SetConstantBuffer(
	const std::string& Name)
{
	mBuffer = CShaderManager::GetInst()->FindCBuffer(Name);
}
