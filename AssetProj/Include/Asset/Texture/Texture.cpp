#include "Texture.h"
#include "Device.h"

#ifdef _DEBUG

#pragma comment(lib, "../Bin/Debug/DirectXTex_Debug.lib")

#else

#pragma comment(lib, "../Bin/Release/DirectXTex.lib")

#endif // _DEBUG


CTexture::CTexture()
{
	mAssetType = EAssetType::Texture;
}

CTexture::~CTexture()
{
	size_t	Size = mTextureList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(mTextureList[i]);
	}
}

bool CTexture::LoadTexture(const TCHAR* FileName)
{
	TCHAR	FullPath[MAX_PATH] = {};

	lstrcpy(FullPath, gRootPath);
	lstrcat(FullPath, TEXT("..\\Asset\\"));
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(FullPath);
}

bool CTexture::LoadTextureFullPath(const TCHAR* FullPath)
{
	FTextureInfo* Texture = new FTextureInfo;

	int	Length = lstrlen(FullPath);

	for (int i = 0; i < Length; ++i)
	{
		if (gRootPath[i] != FullPath[i])
		{
			lstrcpy(Texture->FileName, &FullPath[i]);
			break;
		}
	}

	// 파일 확장자를 구분하여 로딩한다.
	TCHAR	FileExt[_MAX_EXT] = {};

	_wsplitpath_s(FullPath, nullptr, 0, nullptr, 0,
		nullptr, 0, FileExt, _MAX_EXT);

	char	ConvertExt[_MAX_EXT] = {};

	// 유니코드 문자열을 멀티바이트 문자열로 변환
	int Count = WideCharToMultiByte(CP_ACP, 0, FileExt,
		-1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, FileExt, -1,
		ConvertExt, Count, nullptr, nullptr);

	// 대문자로 변경
	_strupr_s(ConvertExt);

	DirectX::ScratchImage* Image = new DirectX::ScratchImage;

	// 확장자 체크
	if (strcmp(ConvertExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(FullPath,
			DirectX::DDS_FLAGS_NONE, nullptr,
			*Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);
			return false;
		}
	}

	else if (strcmp(ConvertExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(FullPath,
			nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);
			return false;
		}
	}

	else
	{
		if (FAILED(DirectX::LoadFromWICFile(FullPath,
			DirectX::WIC_FLAGS_NONE, nullptr,
			*Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);
			return false;
		}
	}

	Texture->Image = Image;

	int	RSVNumber = (int)mTextureList.size();

	mTextureList.emplace_back(Texture);

	return CreateResourceView(RSVNumber);
}

bool CTexture::LoadTexture(
	const std::vector<const TCHAR*>& FileName)
{
	size_t	Size = FileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!LoadTexture(FileName[i]))
			return false;
	}

	return true;
}

bool CTexture::LoadTextureFullPath(
	const std::vector<const TCHAR*>& FullPath)
{
	size_t	Size = FullPath.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!LoadTextureFullPath(FullPath[i]))
			return false;
	}

	return true;
}

bool CTexture::LoadTexture(const TCHAR* FileName, 
	const TCHAR* Ext, int Count)
{
	for (int i = 0; i < Count; ++i)
	{
		TCHAR	_FileName[MAX_PATH] = {};

		wsprintf(_FileName, TEXT("%s%d.%s"), FileName,
			i, Ext);

		if (!LoadTexture(_FileName))
			return false;
	}

	return true;
}

void CTexture::SetShader(int Register, 
	int ShaderBufferType, int TextureIndex)
{
	if (ShaderBufferType & EShaderBufferType::Vertex)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);

	if (ShaderBufferType & EShaderBufferType::Pixel)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);

	if (ShaderBufferType & EShaderBufferType::Hull)
		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);

	if (ShaderBufferType & EShaderBufferType::Domain)
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);

	if (ShaderBufferType & EShaderBufferType::Geometry)
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);

	if (ShaderBufferType & EShaderBufferType::Compute)
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
}

void CTexture::ResetShader(int Register, 
	int ShaderBufferType)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if (ShaderBufferType & EShaderBufferType::Vertex)
		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &SRV);

	if (ShaderBufferType & EShaderBufferType::Pixel)
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &SRV);

	if (ShaderBufferType & EShaderBufferType::Hull)
		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &SRV);

	if (ShaderBufferType & EShaderBufferType::Domain)
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &SRV);

	if (ShaderBufferType & EShaderBufferType::Geometry)
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &SRV);

	if (ShaderBufferType & EShaderBufferType::Compute)
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &SRV);
}

bool CTexture::CreateResourceView(int Index)
{
	if (FAILED(DirectX::CreateShaderResourceView(
		CDevice::GetInst()->GetDevice(),
		mTextureList[Index]->Image->GetImages(),
		mTextureList[Index]->Image->GetImageCount(),
		mTextureList[Index]->Image->GetMetadata(),
		&mTextureList[Index]->SRV)))
		return false;

	mTextureList[Index]->Width =
		(unsigned int)mTextureList[Index]->Image->GetImages()[0].width;
	mTextureList[Index]->Height =
		(unsigned int)mTextureList[Index]->Image->GetImages()[0].height;

	return true;
}
