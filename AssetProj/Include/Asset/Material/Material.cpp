#include "Material.h"
#include "Shader/ShaderManager.h"
#include "../Texture/Texture.h"
#include "../Texture/TextureManager.h"
#include "../AssetManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneAssetManager.h"
#include "Shader/MaterialCBuffer.h"
#include "Device.h"

ID3D11SamplerState* CMaterial::mSampler[ETextureSamplerType::End];

CMaterial::CMaterial()
{
	mCBuffer = new CMaterialCBuffer;

	mCBuffer->Init();
}

CMaterial::CMaterial(const CMaterial& Material)
{
	mCBuffer = new CMaterialCBuffer;

	mCBuffer->Init();

	mTextureList.clear();

	mPS = Material.mPS;
	mBaseColor = Material.mBaseColor;
	mOpacity = Material.mOpacity;
	mSamplerType = Material.mSamplerType;

	mCBuffer->SetOpacity(mOpacity);
	mCBuffer->SetBaseColor(mBaseColor);

	size_t	Size = Material.mTextureList.size();
	
	for (size_t i = 0; i < Size; ++i)
	{
		FMaterialTextureInfo* Info = new FMaterialTextureInfo;

		*Info = *Material.mTextureList[i];

		mTextureList.emplace_back(Info);
	}

	if (!mTextureList.empty())
	{
		mCBuffer->SetTextureSize(
			mTextureList[0]->Texture->GetTexture(0)->Width,
			mTextureList[0]->Texture->GetTexture(0)->Height);
	}

	mRefCount = 0;
}

CMaterial::~CMaterial()
{
	SAFE_DELETE(mCBuffer);

	size_t	Size = mTextureList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(mTextureList[i]);
	}
}

void CMaterial::SetSampler(ETextureSamplerType::Type Type)
{
	D3D11_SAMPLER_DESC	Desc = {};

	switch (Type)
	{
	case ETextureSamplerType::Point:
		Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case ETextureSamplerType::Linear:
		Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case ETextureSamplerType::Anisotropic:
		Desc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}

	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	Desc.MipLODBias = 0.f;
	Desc.MaxAnisotropy = 1;
	Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Desc.MinLOD = -FLT_MAX;
	Desc.MaxLOD = FLT_MAX;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(
		&Desc, &mSampler[Type])))
		return;
}

void CMaterial::DestroySampler()
{
	for (int i = 0; i < ETextureSamplerType::End; ++i)
	{
		SAFE_RELEASE(mSampler[i]);
	}
}

void CMaterial::AddTexture(CTexture* Texture,
	int Register,
	int ShaderBufferType, int TextureIndex)
{
	FMaterialTextureInfo* TexInfo = new FMaterialTextureInfo;

	TexInfo->Texture = Texture;
	TexInfo->Name = Texture->GetName();
	TexInfo->Register = Register;
	TexInfo->ShaderBufferType = ShaderBufferType;
	TexInfo->TextureIndex = TextureIndex;

	mCBuffer->SetTextureSize(
		TexInfo->Texture->GetTexture(TextureIndex)->Width,
		TexInfo->Texture->GetTexture(TextureIndex)->Height);

	mTextureList.emplace_back(TexInfo);
}

void CMaterial::AddTexture(const std::string& Name, 
	int Register,
	int ShaderBufferType, int TextureIndex)
{
	FMaterialTextureInfo* TexInfo = new FMaterialTextureInfo;

	if (mScene)
		TexInfo->Texture = mScene->GetAssetManager()->FindTexture(Name);

	else
		TexInfo->Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);

	TexInfo->Name = TexInfo->Texture->GetName();
	TexInfo->Register = Register;
	TexInfo->ShaderBufferType = ShaderBufferType;
	TexInfo->TextureIndex = TextureIndex;

	mCBuffer->SetTextureSize(
		TexInfo->Texture->GetTexture(TextureIndex)->Width,
		TexInfo->Texture->GetTexture(TextureIndex)->Height);

	mTextureList.emplace_back(TexInfo);
}

void CMaterial::AddTexture(const std::string& Name,
	const TCHAR* FileName, 
	int Register, int ShaderBufferType, int TextureIndex)
{
	FMaterialTextureInfo* TexInfo = new FMaterialTextureInfo;

	if (mScene)
	{
		if (!mScene->GetAssetManager()->LoadTexture(Name, FileName))
		{
			SAFE_DELETE(TexInfo);
			return;
		}
		TexInfo->Texture = mScene->GetAssetManager()->FindTexture(Name);
	}

	else
	{
		if (!CAssetManager::GetInst()->GetTextureManager()->LoadTexture(Name, FileName))
		{
			SAFE_DELETE(TexInfo);
			return;
		}
		TexInfo->Texture = CAssetManager::GetInst()->GetTextureManager()->FindTexture(Name);
	}

	TexInfo->Name = TexInfo->Texture->GetName();
	TexInfo->Register = Register;
	TexInfo->ShaderBufferType = ShaderBufferType;
	TexInfo->TextureIndex = TextureIndex;

	mCBuffer->SetTextureSize(
		TexInfo->Texture->GetTexture(TextureIndex)->Width,
		TexInfo->Texture->GetTexture(TextureIndex)->Height);

	mTextureList.emplace_back(TexInfo);
}

void CMaterial::SetPixelShader(const std::string& Name)
{
	const FMaterialPixelShader* mps = CShaderManager::GetInst()->FindPixelShader(Name);

	if (!mps)
		return;

	mPS = mps->PS;
}

void CMaterial::SetPixelShader(const std::string& Name,
	const char* EntryName, const TCHAR* FileName)
{
	if (!CShaderManager::GetInst()->LoadPixelShader(Name,
		EntryName, FileName))
		return;

	const FMaterialPixelShader* mps = CShaderManager::GetInst()->FindPixelShader(Name);

	mPS = mps->PS;
}

void CMaterial::ClearShader()
{
	mPS = nullptr;
}

void CMaterial::SetBaseColor(float r, float g, 
	float b, float a)
{
	mBaseColor = FVector4D(r, g, b, a);

	mCBuffer->SetBaseColor(mBaseColor);
}

void CMaterial::SetBaseColor(const FVector4D& Color)
{
	mBaseColor = Color;

	mCBuffer->SetBaseColor(Color);
}

void CMaterial::SetOpacity(float Opacity)
{
	mOpacity = Opacity;

	mCBuffer->SetOpacity(Opacity);
}

void CMaterial::SetSamplerType(
	ETextureSamplerType::Type SamplerType)
{
	mSamplerType = SamplerType;
}

void CMaterial::SetMaterial()
{
	mCBuffer->UpdateBuffer();

	CDevice::GetInst()->GetContext()->PSSetSamplers(0,
		1, &mSampler[mSamplerType]);

	if (mPS)
		CDevice::GetInst()->GetContext()->PSSetShader(mPS, nullptr, 0);

	size_t	TexCount = mTextureList.size();

	for (size_t i = 0; i < TexCount; ++i)
	{
		mTextureList[i]->Texture->SetShader(mTextureList[i]->Register,
			mTextureList[i]->ShaderBufferType,
			mTextureList[i]->TextureIndex);
	}
}

void CMaterial::ResetMaterial()
{
	size_t	TexCount = mTextureList.size();

	for (size_t i = 0; i < TexCount; ++i)
	{
		mTextureList[i]->Texture->ResetShader(mTextureList[i]->Register,
			mTextureList[i]->ShaderBufferType);
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}
