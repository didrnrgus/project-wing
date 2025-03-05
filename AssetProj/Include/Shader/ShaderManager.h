#pragma once

#include "GameInfo.h"
#include "Shader.h"

struct FMaterialPixelShader
{
	ID3DBlob* Blob = nullptr;
	ID3D11PixelShader* PS = nullptr;

	~FMaterialPixelShader()
	{
		SAFE_RELEASE(Blob);
		SAFE_RELEASE(PS);
	}
};

class CShaderManager
{
private:
	std::unordered_map<std::string, CSharedPtr<CShader>>	mShaderMap;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>>	mCBufferMap;
	std::unordered_map<std::string, FMaterialPixelShader*>	mPixelShaderMap;

public:
	bool Init();
	bool LoadPixelShader(const std::string& Name,
		const char* EntryName, 
		const TCHAR* FileName);
	const FMaterialPixelShader* FindPixelShader(const std::string& Name);
	CShader* FindShader(const std::string& Name);
	void ReleaseShader(const std::string& Name);

	template <typename T>
	bool CreateShader(const std::string& Name)
	{
		CShader* Shader = FindShader(Name);

		if (Shader)
			return true;

		Shader = new T;

		if (!Shader->Init())
		{
			SAFE_DELETE(Shader);
			return false;
		}

		mShaderMap.insert(std::make_pair(Name, Shader));

		return true;
	}

public:
	bool CreateConstantBuffer(const std::string& Name,
		int Size, int Register,
		int ShaderBufferType = EShaderBufferType::Graphic);
	CConstantBuffer* FindCBuffer(const std::string& Name);
	void ReleaseCBuffer(const std::string& Name);

	DECLARE_SINGLE(CShaderManager)
};

