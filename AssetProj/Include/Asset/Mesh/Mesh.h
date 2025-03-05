#pragma once

#include "../Asset.h"

struct FMeshSlot
{
    FIndexBuffer    IndexBuffer;
    CSharedPtr<class CMaterial> Material;
};

class CMesh abstract :
    public CAsset
{
public:
    CMesh();
    virtual ~CMesh();

protected:
    FVertexBuffer       mVertexBuffer;
    std::vector<FMeshSlot*> mMeshSlot;
    D3D11_PRIMITIVE_TOPOLOGY    mPrimitive;

public:
    int GetSlotCount()  const
    {
        return (int)mMeshSlot.size();
    }

    const FMeshSlot* GetSlot(int SlotIndex) const
    {
        return mMeshSlot[SlotIndex];
    }

public:
    virtual bool CreateMesh(void* VertexData, int Size, int Count,
        D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
        void* IndexData = nullptr, int IndexSize = 0, int IndexCount = 0,
        DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

public:
    bool CreateBuffer(ID3D11Buffer** Buffer, D3D11_BIND_FLAG Flag, void* Data, 
        int Size, int Count, D3D11_USAGE Usage);
    void SetMaterial(int SlotIndex, const std::string& Name);
    void SetMaterial(int SlotIndex, class CMaterial* Material);

public:
    virtual void Render();
    virtual void Render(int SlotIndex);
};

