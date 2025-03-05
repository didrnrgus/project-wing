#pragma once

#include "RenderState.h"

class CBlendState :
    public CRenderState
{
	friend class CRenderStateManager;

protected:
	CBlendState();
	virtual ~CBlendState();

protected:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC>	mDescs;
    float       mBlendFactor[4] = {};
    float       mPrevBlendFactor[4] = {};
    UINT        mSampleMask = 0xffffffff;
    UINT        mPrevSampleMask = 0;

public:
    void SetBlendFactor(float r, float g, float b, float a);
    void SetSampleMask(UINT Mask);
	// Src : 출력물체 색상
	// Dest : 백버퍼 색상
	void AddBlendDesc(bool Enable, D3D11_BLEND Src,
		D3D11_BLEND Dest, D3D11_BLEND_OP BlendOp,
		D3D11_BLEND SrcAlpha, D3D11_BLEND DestAlpha,
		D3D11_BLEND_OP AlphaOp,
		UINT8 RenderTargetWriteMask);
    bool CreateState(bool AlphaToCoverageEnable,
        bool IndependentBlendEnable);
	/*	
        D3D11_BLEND_ZERO	= 1, (0, 0, 0)
        D3D11_BLEND_ONE	= 2, (1, 1, 1)
        D3D11_BLEND_SRC_COLOR	= 3, 물체색상
        D3D11_BLEND_INV_SRC_COLOR	= 4, 1 - 물체색상
        D3D11_BLEND_SRC_ALPHA	= 5, 물체알파
        D3D11_BLEND_INV_SRC_ALPHA	= 6, 1 - 물체알파
        D3D11_BLEND_DEST_ALPHA	= 7,
        D3D11_BLEND_INV_DEST_ALPHA	= 8,
        D3D11_BLEND_DEST_COLOR	= 9,
        D3D11_BLEND_INV_DEST_COLOR	= 10,
        D3D11_BLEND_SRC_ALPHA_SAT	= 11,
        D3D11_BLEND_BLEND_FACTOR	= 14,
        D3D11_BLEND_INV_BLEND_FACTOR	= 15,
        D3D11_BLEND_SRC1_COLOR	= 16,
        D3D11_BLEND_INV_SRC1_COLOR	= 17,
        D3D11_BLEND_SRC1_ALPHA	= 18,
        D3D11_BLEND_INV_SRC1_ALPHA	= 19
	*/

public:
	virtual void SetState();
	virtual void ResetState();
};

