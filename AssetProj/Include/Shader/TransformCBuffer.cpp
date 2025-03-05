#include "TransformCBuffer.h"

CTransformCBuffer::CTransformCBuffer()
{
}

CTransformCBuffer::CTransformCBuffer(const CTransformCBuffer& Data) :
    CConstantBufferData(Data),
    mData(Data.mData)
{
}

CTransformCBuffer::CTransformCBuffer(CTransformCBuffer&& Data) :
    CConstantBufferData(Data),
    mData(Data.mData)
{
}

CTransformCBuffer::~CTransformCBuffer()
{
}

bool CTransformCBuffer::Init()
{
    SetConstantBuffer("Transform");

    return true;
}

void CTransformCBuffer::UpdateBuffer()
{
    mData.matWV = mData.matWorld * mData.matView;
    mData.matWVP = mData.matWV * mData.matProj;

    mData.matWorld.Transpose();
    mData.matView.Transpose();
    mData.matProj.Transpose();
    mData.matWV.Transpose();
    mData.matWVP.Transpose();

    mBuffer->Update(&mData);
}

CTransformCBuffer* CTransformCBuffer::Clone()
{
    return new CTransformCBuffer(*this);
}
