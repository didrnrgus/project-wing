#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m2DTarget);
	SAFE_RELEASE(m2DFacotry);

	SAFE_RELEASE(mTargetView);
	SAFE_RELEASE(mDepthView);

	SAFE_RELEASE(mSwapChain);

	if (mContext)
		mContext->ClearState();

	SAFE_RELEASE(mContext);

	SAFE_RELEASE(mDevice);
}

FVector2D CDevice::GetResolutionRatio() const
{
	// 윈도우 클라이언트 영역의 크기를 얻어온다.
	RECT	WindowRC;

	GetClientRect(mhWnd, &WindowRC);

	float Width = (float)WindowRC.right - WindowRC.left;
	float Height = (float)WindowRC.bottom - WindowRC.top;

	return FVector2D(mRS.Width / Width, mRS.Height / Height);
}

bool CDevice::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool WindowMode)
{
	mhWnd = hWnd;
	mRS.Width = Width;
	mRS.Height = Height;
	mWindowMode = WindowMode;

	unsigned int	Flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	Flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_FEATURE_LEVEL	FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL	FLevelResult;

	// Device와 DeviceContext를 생성한다.
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &FLevel,
		1, D3D11_SDK_VERSION, &mDevice, &FLevelResult, &mContext)))
	{
		return false;
	}

	unsigned int	SampleCount = 4;

	// 생성된 Device를 이용하여 지원되는 멀티샘플링 수를 얻어온다.
	unsigned int	Check = 0;
	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
		&Check);

	if (Check < 1)
		SampleCount = 1;

	SampleCount = 1;

	// SwapChain을 생성한다.
	DXGI_SWAP_CHAIN_DESC	SwapDesc = {};

	// BufferDesc : 백버퍼에 대한 정의.
	// 백버퍼의 가로 해상도를 지정한다.
	SwapDesc.BufferDesc.Width = Width;

	// 백버퍼의 세로 해상도를 지정한다.
	SwapDesc.BufferDesc.Height = Height;

	// 백버퍼의 픽셀 포맷을 지정한다.
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 화면 주사율 지정.
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;

	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapDesc.BufferDesc.ScanlineOrdering =
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// 백버퍼 수를 지정한다.
	SwapDesc.BufferCount = 1;

	// 버퍼의 용도를 지정한다.
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 출력할 윈도우 핸들을 지정한다.
	SwapDesc.OutputWindow = hWnd;

	// 멀티샘플링(MSAA : Multi Sample Anti Aliasing)
	SwapDesc.SampleDesc.Quality = 0;
	SwapDesc.SampleDesc.Count = SampleCount;

	// 창모드인지 풀스크린 모드인지 지정한다.
	SwapDesc.Windowed = WindowMode;

	// SwapChain이 버퍼를 전환할 때 사용하는 동작을 지정한다.
	// DXGI_SWAP_EFFECT_DISCARD : 이전 내용을 무시하고 전환한다.
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice* DXGIDevice = nullptr;

	// 객체를 얻어오면 내부에서 AddRef를 해둔다.
	mDevice->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&DXGIDevice);

	IDXGIAdapter* Adapter = nullptr;
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&Adapter);

	IDXGIFactory* Factory = nullptr;
	Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&Factory);

	if (FAILED(Factory->CreateSwapChain(mDevice, &SwapDesc, &mSwapChain)))
	{
		SAFE_RELEASE(Factory);
		SAFE_RELEASE(Adapter);
		SAFE_RELEASE(DXGIDevice);
		return false;
	}

	SAFE_RELEASE(Factory);
	SAFE_RELEASE(Adapter);
	SAFE_RELEASE(DXGIDevice);

	// SwapChain에 생성된 백버퍼를 얻어온다.
	ID3D11Texture2D* BackBuffer = nullptr;

	// GetBuffer를 하며 버퍼를 얻어오면 AddRef가 호출된 상태로 얻어온다.
	if (FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer)))
	{
		return false;
	}

	// 백버퍼에 출력하기 위한 렌더타겟을 만든다.
	if (FAILED(mDevice->CreateRenderTargetView(BackBuffer, nullptr, &mTargetView)))
	{
		return false;
	}

	// RefCount를 감소시킨다.
	SAFE_RELEASE(BackBuffer);

	// 깊이.스텐실 버퍼를 만들어주기 위한 Texture를 생성한다.
	D3D11_TEXTURE2D_DESC	DepthDesc = {};

	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.ArraySize = 1;

	// D24 : 깊이에 24비트를 UNORM(0 ~ 1)로 쓰겠다.
	// S8 : 스텐실에 8비트를 UINT(0 ~ 255)로 쓰겠다.
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;

	DepthDesc.SampleDesc.Count = SampleCount;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.MipLevels = 1;

	ID3D11Texture2D* DepthBuffer = nullptr;

	// 위에서 정의한 깊이버퍼용 Texture를 만든다.
	if (FAILED(mDevice->CreateTexture2D(&DepthDesc, nullptr, &DepthBuffer)))
		return false;

	// 깊이버퍼를 만든다.
	if (FAILED(mDevice->CreateDepthStencilView(DepthBuffer, nullptr, &mDepthView)))
		return false;

	SAFE_RELEASE(DepthBuffer);

	D3D11_VIEWPORT	VP = {};

	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	mContext->RSSetViewports(1, &VP);

	// Font 출력을 위한 2D 초기화
	// D2D1_FACTORY_TYPE_SINGLE_THREADED : 싱글스레드 환경
	// D2D1_FACTORY_TYPE_MULTI_THREADED : 멀티스레드 환경
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m2DFacotry)))
		return false;

	// 위에서 생성한 3D Backbuffer와 2D BackBuffer를 연결한다.
	// 픽셀을 저장하는 모든 객체는 Surface라는것을 들고 있다.
	// 3D BackBuffer의 Surface를 얻어온다.
	IDXGISurface* BackSurface = nullptr;

	mSwapChain->GetBuffer(0, IID_PPV_ARGS(&BackSurface));

	// 렌더타겟을 생성한다. 이 렌더타겟이 출력하는 Surface를 위의 Surface로
	// 지정하면 2D 출력 시 지정된 Surface에 출력하게 되어
	// BackBuffer에 출력을 하게 된다.
	D2D1_RENDER_TARGET_PROPERTIES	prop =
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, 
				D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m2DFacotry->CreateDxgiSurfaceRenderTarget(BackSurface,
		prop, &m2DTarget)))
	{
		SAFE_RELEASE(BackSurface);
		return false;
	}

	SAFE_RELEASE(BackSurface);

	return true;
}

void CDevice::ClearBackBuffer(float ClearColor[4])
{
	mContext->ClearRenderTargetView(mTargetView, ClearColor);
}

void CDevice::ClearDepthStencil(float Depth, unsigned char Stencil)
{
	mContext->ClearDepthStencilView(mDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		Depth, Stencil);
}

void CDevice::SetTarget()
{
	// 출력시 값을 저장할 렌더타겟과 뎁스뷰를 지정한다.
	mContext->OMSetRenderTargets(1, &mTargetView, mDepthView);
}

void CDevice::Render()
{
	mSwapChain->Present(0, 0);
}
