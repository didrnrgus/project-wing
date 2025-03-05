#include "FrameMeshShader.h"

CFrameMeshShader::CFrameMeshShader()
{
}

CFrameMeshShader::~CFrameMeshShader()
{
}

bool CFrameMeshShader::Init()
{
    if (!LoadVertexShader("FrameMeshVS", TEXT("ColorMesh.fx")))
        return false;

    if (!LoadPixelShader("FrameMeshPS", TEXT("ColorMesh.fx")))
        return false;

    AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
        0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
