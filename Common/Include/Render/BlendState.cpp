#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()
{
}

CBlendState::~CBlendState()
{
}

void CBlendState::SetBlendFactor(float r, float g, 
    float b, float a)
{
    mBlendFactor[0] = r;
    mBlendFactor[1] = g;
    mBlendFactor[2] = b;
    mBlendFactor[3] = a;
}

void CBlendState::SetSampleMask(UINT Mask)
{
    mSampleMask = Mask;
}

void CBlendState::AddBlendDesc(bool Enable,
	D3D11_BLEND Src, D3D11_BLEND Dest, 
	D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcAlpha,
	D3D11_BLEND DestAlpha, D3D11_BLEND_OP AlphaOp,
	UINT8 RenderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC	Desc = {};

    Desc.BlendEnable = Enable;
    Desc.SrcBlend = Src;
    Desc.DestBlend = Dest;
    Desc.BlendOp = BlendOp;
    Desc.SrcBlendAlpha = SrcAlpha;
    Desc.DestBlendAlpha = DestAlpha;
    Desc.BlendOpAlpha = AlphaOp;
    Desc.RenderTargetWriteMask = RenderTargetWriteMask;

    mDescs.emplace_back(Desc);
}

bool CBlendState::CreateState(bool AlphaToCoverageEnable,
    bool IndependentBlendEnable)
{
    if (mDescs.empty())
        return false;

    D3D11_BLEND_DESC    Desc = {};

    Desc.AlphaToCoverageEnable = AlphaToCoverageEnable;
    Desc.IndependentBlendEnable = IndependentBlendEnable;

    // RenderTargetDesc의 기본값을 설정한다.
    for (int i = 0; i < 8; ++i)
    {
        Desc.RenderTarget[i].BlendEnable = false;
        Desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
        Desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        Desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        Desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        Desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        Desc.RenderTarget[i].RenderTargetWriteMask = 
            D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    memcpy(Desc.RenderTarget, &mDescs[0],
        sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * mDescs.size());

    if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(
        &Desc, (ID3D11BlendState**)&mState)))
        return false;

    return true;
}

void CBlendState::SetState()
{
    // 현재 디바이스에 지정된 블렌드 스테이트를 얻어온다.
    // Get 해서 얻어온 State는 레퍼런스 카운트가 1 증가되어
    // 있다.
    CDevice::GetInst()->GetContext()->OMGetBlendState(
        (ID3D11BlendState**)&mPrevState, mPrevBlendFactor,
        &mPrevSampleMask);

    CDevice::GetInst()->GetContext()->OMSetBlendState(
        (ID3D11BlendState*)mState, mBlendFactor,
        mSampleMask);
}

void CBlendState::ResetState()
{
    CDevice::GetInst()->GetContext()->OMSetBlendState(
        (ID3D11BlendState*)mPrevState, mPrevBlendFactor,
        mPrevSampleMask);

    SAFE_RELEASE(mPrevState);
}
