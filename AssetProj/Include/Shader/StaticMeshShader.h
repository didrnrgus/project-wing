#pragma once

#include "GraphicShader.h"

class CStaticMeshShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CStaticMeshShader();
    virtual ~CStaticMeshShader();

public:
    virtual bool Init();
};

