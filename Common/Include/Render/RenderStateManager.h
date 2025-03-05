#pragma once

#include "../GameInfo.h"

class CRenderStateManager
{
	friend class CRenderManager;

private:
	CRenderStateManager();
	~CRenderStateManager();

private:
	std::unordered_map<std::string, class CRenderState*> mRenderStateMap;

public:
	bool Init();

public:
	void SetBlendFactor(const std::string& Name, 
		float r, float g, float b, float a);
	void SetSampleMask(const std::string& Name, UINT Mask);
	void AddBlendDesc(const std::string& Name,
		bool Enable, D3D11_BLEND Src,
		D3D11_BLEND Dest, D3D11_BLEND_OP BlendOp,
		D3D11_BLEND SrcAlpha, D3D11_BLEND DestAlpha,
		D3D11_BLEND_OP AlphaOp,
		UINT8 RenderTargetWriteMask);
	bool CreateBlendState(const std::string& Name,
		bool AlphaToCoverageEnable,
		bool IndependentBlendEnable);

	bool CreateDepthStencilState(const std::string& Name,
		bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC FrontFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC BackFace =
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });

public:
	class CRenderState* FindState(const std::string& Name);
};

