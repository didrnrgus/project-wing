#pragma once

#include "../Asset.h"
#include "DirectXTex.h"

struct FTextureInfo
{
    DirectX::ScratchImage* Image = nullptr;
    ID3D11ShaderResourceView* SRV = nullptr;
    unsigned int    Width = 0;
    unsigned int    Height = 0;
    TCHAR           FileName[MAX_PATH] = {};

    ~FTextureInfo()
    {
        SAFE_RELEASE(SRV);
        SAFE_DELETE(Image);
    }
};

class CTexture :
    public CAsset
{
    friend class CTextureManager;

protected:
    CTexture();
    virtual ~CTexture();

protected:
    std::vector<FTextureInfo*>  mTextureList;

public:
    const FTextureInfo* GetTexture(int Index = 0)
    {
        return mTextureList[Index];
    }

    int GetTextureCount()   const
    {
        return (int)mTextureList.size();
    }

public:
    bool LoadTexture(const TCHAR* FileName);
    bool LoadTextureFullPath(const TCHAR* FullPath);
    bool LoadTexture(const std::vector<const TCHAR*>& FileName);
    bool LoadTextureFullPath(const std::vector<const TCHAR*>& FullPath);
    bool LoadTexture(const TCHAR* FileName, const TCHAR* Ext,
        int Count);

public:
    void SetShader(int Register, int ShaderBufferType,
        int TextureIndex);
    void ResetShader(int Register, int ShaderBufferType);

protected:
    bool CreateResourceView(int Index = 0);
};

