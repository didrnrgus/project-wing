#pragma once

#include "Shader.h"

class CGraphicShader abstract :
    public CShader
{
    friend class CShaderManager;

protected:
    CGraphicShader();
    virtual ~CGraphicShader();

private:
    ID3D11InputLayout* mInputLayout = nullptr;

    // 레지스터 지정을 위한 구조체
    std::vector<D3D11_INPUT_ELEMENT_DESC>   mvecDesc;

    unsigned int    mInputLayoutSize = 0;

    // VertexShader 객체.
    ID3D11VertexShader* mVS = nullptr;

    // VertexShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ID3DBlob* mVSBlob = nullptr;

    // PixelShader 객체.
    ID3D11PixelShader* mPS = nullptr;

    // PixelShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ID3DBlob* mPSBlob = nullptr;

    // HullShader 객체.
    ID3D11HullShader* mHS = nullptr;

    // HullShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ID3DBlob* mHSBlob = nullptr;

    // DomainShader 객체.
    ID3D11DomainShader* mDS = nullptr;

    // DomainShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ID3DBlob* mDSBlob = nullptr;

    // GeometryShader 객체.
    ID3D11GeometryShader* mGS = nullptr;

    // GeometryShader 코드를 컴파일하고 바이트 코드를 저장하기 위한 버퍼
    ID3DBlob* mGSBlob = nullptr;

public:
    virtual bool Init();
    virtual void SetShader() final;

public:
    void AddInputLayoutDesc(const char* Semantic, unsigned int SemanticIndex,
        DXGI_FORMAT Fmt, unsigned int InputSlot, unsigned int Size,
        D3D11_INPUT_CLASSIFICATION InputSlotClass,
        unsigned int InstanceDataStepRate);
    bool CreateInputLayout();
    bool LoadVertexShader(const char* EntryName, const TCHAR* FileName);
    bool LoadPixelShader(const char* EntryName, const TCHAR* FileName);
    bool LoadHullShader(const char* EntryName, const TCHAR* FileName);
    bool LoadDomainShader(const char* EntryName, const TCHAR* FileName);
    bool LoadGeometryShader(const char* EntryName, const TCHAR* FileName);
};

