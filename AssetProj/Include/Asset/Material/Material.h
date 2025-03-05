#pragma once

#include "../Asset.h"

namespace ETextureSamplerType
{
    enum Type : unsigned char
    {
        Point,
        Linear,
        Anisotropic,
        End
    };
}

struct FMaterialTextureInfo
{
    std::string         Name;
    CSharedPtr<class CTexture>  Texture;
    int                 Register = 0;
    int                 ShaderBufferType = EShaderBufferType::Pixel;
    int                 TextureIndex = 0;
};

class CMaterial :
    public CAsset
{
    friend class CMaterialManager;

protected:
    CMaterial();
    CMaterial(const CMaterial& Material);
    virtual ~CMaterial();

private:
    static ID3D11SamplerState* mSampler[ETextureSamplerType::End];

private:
    static void SetSampler(ETextureSamplerType::Type Type);
    static void DestroySampler();

protected:
    std::vector<FMaterialTextureInfo*> mTextureList;
    ID3D11PixelShader* mPS = nullptr;
    FVector4D          mBaseColor = FVector4D::White;
    // 0 ~ 1 사이의 불투명도를 지정한다.
    // 1 : 불투명 0 : 완전투명
    float               mOpacity = 1.f;
    class CMaterialCBuffer* mCBuffer = nullptr;
    ETextureSamplerType::Type mSamplerType = ETextureSamplerType::Linear;

public:
    void AddTexture(class CTexture* Texture,
        int Register,
        int ShaderBufferType = EShaderBufferType::Pixel,
        int TextureIndex = 0);
    void AddTexture(const std::string& Name,
        int Register,
        int ShaderBufferType = EShaderBufferType::Pixel,
        int TextureIndex = 0);
    void AddTexture(const std::string& Name,
        const TCHAR* FileName,
        int Register,
        int ShaderBufferType = EShaderBufferType::Pixel,
        int TextureIndex = 0);
    void SetPixelShader(const std::string& Name);
    void SetPixelShader(const std::string& Name,
        const char* EntryName, const TCHAR* FileName);
    void ClearShader();
    void SetBaseColor(float r, float g, float b, float a);
    void SetBaseColor(const FVector4D& Color);
    void SetOpacity(float Opacity);
    void SetSamplerType(ETextureSamplerType::Type SamplerType);
    void SetMaterial();
    void ResetMaterial();
    CMaterial* Clone();
};

