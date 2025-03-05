#pragma once

#include "Share/Object.h"

class CShader abstract :
    public CObject
{
    friend class CShaderManager;

protected:
    CShader();
    virtual ~CShader();

public:
    virtual bool Init() = 0;
    virtual void SetShader() = 0;
};

