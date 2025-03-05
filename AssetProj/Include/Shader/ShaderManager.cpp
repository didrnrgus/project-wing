#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "FrameMeshShader.h"
#include "ConstantBuffer.h"
#include "StaticMeshShader.h"
#include "Device.h"
#include "SpriteShader.h"
#include "UIShader.h"
#include "TileMapShader.h"
#include "TileShader.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
	auto	iter = mPixelShaderMap.begin();
	auto	iterEnd = mPixelShaderMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CShaderManager::Init()
{
	CreateShader<CColorMeshShader>("ColorMeshShader");

	CreateShader<CFrameMeshShader>("FrameMeshShader");

	CreateShader<CStaticMeshShader>("StaticMeshShader");

	CreateShader<CSpriteShader>("SpriteShader");

	CreateShader<CUIShader>("UIShader");

	CreateShader<CTileMapShader>("TileMapShader");

	CreateShader<CTileShader>("TileShader");



	if (!LoadPixelShader("DefaultMaterialShader",
		"DefaultMaterialPS", TEXT("Mesh.fx")))
		return false;


	CreateConstantBuffer("Transform",
		sizeof(FTransformCBufferInfo),
		0, EShaderBufferType::Vertex);

	CreateConstantBuffer("Material",
		sizeof(FMaterialCBufferInfo),
		1, EShaderBufferType::Pixel);

	CreateConstantBuffer("Animation2D",
		sizeof(FAnimation2DCBufferInfo),
		2, EShaderBufferType::Vertex);

	CreateConstantBuffer("Collider",
		sizeof(FColliderCBufferInfo),
		3, EShaderBufferType::Pixel);

	CreateConstantBuffer("Sprite",
		sizeof(FSpriteCBufferInfo),
		3, EShaderBufferType::Pixel);

	CreateConstantBuffer("UI",
		sizeof(FUICBufferInfo),
		3, EShaderBufferType::Vertex | EShaderBufferType::Pixel);

	CreateConstantBuffer("TileMap",
		sizeof(FTileMapCBufferInfo),
		3, EShaderBufferType::Vertex);

	return true;
}

bool CShaderManager::LoadPixelShader(
	const std::string& Name, const char* EntryName,
	const TCHAR* FileName)
{
	if (FindPixelShader(Name))
		return true;

	TCHAR   FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, TEXT("../Bin/Shader/"));
	lstrcat(FullPath, FileName);

	unsigned int    Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	FMaterialPixelShader*	mps = new FMaterialPixelShader;


	ID3DBlob* ErrorBlob = nullptr;

	if (FAILED(D3DCompileFromFile(FullPath, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryName,
		"ps_5_0", Flag, 0, &mps->Blob, &ErrorBlob)))
	{
#ifdef _DEBUG
		char    ErrorText[512] = {};
		strcpy_s(ErrorText, (const char*)ErrorBlob->GetBufferPointer());
		strcat_s(ErrorText, "\n");
		OutputDebugStringA(ErrorText);
#endif // _DEBUG

		return false;
	}

	// mVSBlob->GetBufferPointer() : 컴파일된 코드
	// mVSBlob->GetBufferSize() : 컴파일된 코드의 크기
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(
		mps->Blob->GetBufferPointer(), mps->Blob->GetBufferSize(),
		nullptr, &mps->PS)))
		return false;

	mPixelShaderMap.insert(std::make_pair(Name, mps));

	return true;
}

const FMaterialPixelShader* CShaderManager::FindPixelShader(
	const std::string& Name)
{
	auto	iter = mPixelShaderMap.find(Name);

	if (iter == mPixelShaderMap.end())
		return nullptr;

	return iter->second;
}

CShader* CShaderManager::FindShader(const std::string& Name)
{
	//std::unordered_map<std::string, CSharedPtr>::iterator
	auto iter = mShaderMap.find(Name);

	if (iter == mShaderMap.end())
		return nullptr;

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& Name)
{
	auto iter = mShaderMap.find(Name);

	if (iter != mShaderMap.end())
	{
		mShaderMap.erase(iter);
	}
}

bool CShaderManager::CreateConstantBuffer(
	const std::string& Name, int Size, 
	int Register, int ShaderBufferType)
{
	CConstantBuffer* CBuffer = FindCBuffer(Name);

	if (CBuffer)
		return true;

	CBuffer = new CConstantBuffer;

	if (!CBuffer->Init(Size, Register, ShaderBufferType))
	{
		SAFE_DELETE(CBuffer);
		return false;
	}

	mCBufferMap.insert(std::make_pair(Name, CBuffer));

	return true;
}

CConstantBuffer* CShaderManager::FindCBuffer(
	const std::string& Name)
{
	auto iter = mCBufferMap.find(Name);

	if (iter == mCBufferMap.end())
		return nullptr;

	return iter->second;
}

void CShaderManager::ReleaseCBuffer(const std::string& Name)
{
	auto iter = mCBufferMap.find(Name);

	if (iter != mCBufferMap.end())
	{
		mCBufferMap.erase(iter);
	}
}
