#pragma once

#include "GameInfo.h"

class CDevice
{
private:
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mContext = nullptr;

	// 페이지 플리핑 처리
	IDXGISwapChain* mSwapChain = nullptr;

	ID3D11RenderTargetView* mTargetView = nullptr;
	ID3D11DepthStencilView* mDepthView = nullptr;

	HWND			mhWnd;
	FResolution		mRS;

	bool			mWindowMode = true;

private:
	ID2D1RenderTarget* m2DTarget = nullptr;
	ID2D1Factory* m2DFacotry = nullptr;

public:
	ID2D1RenderTarget* Get2DTarget()	const
	{
		return m2DTarget;
	}

public:
	ID3D11Device* GetDevice()	const
	{
		return mDevice;
	}

	ID3D11DeviceContext* GetContext()	const
	{
		return mContext;
	}

	IDXGISwapChain* GetSwapChain()	const
	{
		return mSwapChain;
	}

	const FResolution& GetResolution()	const
	{
		return mRS;
	}

	bool GetWindowMode()	const
	{
		return mWindowMode;
	}

	FVector2D GetResolutionRatio()	const;

public:
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool WindowMode);
	void ClearBackBuffer(float ClearColor[4]);
	void ClearDepthStencil(float Depth, unsigned char Stencil);
	void SetTarget();
	void Render();

	DECLARE_SINGLE(CDevice)
};

