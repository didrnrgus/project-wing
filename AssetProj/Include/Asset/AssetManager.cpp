#include "AssetManager.h"
#include "Mesh/MeshManager.h"
#include "Texture/TextureManager.h"
#include "Material/MaterialManager.h"
#include "Animation/Animation2DManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"
#include "Asset.h"

DEFINITION_SINGLE(CAssetManager)

CAssetManager::CAssetManager()
{
}

CAssetManager::~CAssetManager()
{
	SAFE_DELETE(mFontManager);
	SAFE_DELETE(mSoundManager);
	SAFE_DELETE(mAnimation2DManager);
	SAFE_DELETE(mMaterialManager);
	SAFE_DELETE(mTextureManager);
	SAFE_DELETE(mMeshManager);
}

bool CAssetManager::Init()
{
	// Root폴더 경로 생성
	// 아래 함수는 실행파일 이름까지 포함한 전체경로를
	// 얻어온다.
	GetModuleFileName(0, gRootPath, MAX_PATH);

	// D:\Lecture\KDT20242\KDT2Framework\KDT2Framework\Bin\KDT2Framework.exe
	int	Length = lstrlen(gRootPath);

	for (int i = Length - 1; i >= 0; --i)
	{
		if (gRootPath[i] == '/' || gRootPath[i] == '\\')
		{
			memset(&gRootPath[i + 1], 0,
				sizeof(TCHAR) * (Length - (i + 1)));
			break;
		}
	}

	// TCHAR -> char
	Length = WideCharToMultiByte(CP_ACP, 0, gRootPath, -1, nullptr, 0,
		nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, gRootPath, -1, 
		gRootPathMultibyte, Length,	nullptr, nullptr);

	mMaterialManager = new CMaterialManager;

	if (!mMaterialManager->Init())
		return false;

	mMeshManager = new CMeshManager;

	if (!mMeshManager->Init())
		return false;

	mTextureManager = new CTextureManager;

	if (!mTextureManager->Init())
		return false;

	mAnimation2DManager = new CAnimation2DManager;

	if (!mAnimation2DManager->Init())
		return false;

	mSoundManager = new CSoundManager;

	if (!mSoundManager->Init())
		return false;

	mFontManager = new CFontManager;

	if (!mFontManager->Init())
		return false;

	return true;
}

void CAssetManager::ReleaseAsset(CAsset* Asset)
{
	switch (Asset->GetAssetType())
	{
	case EAssetType::Mesh:
		mMeshManager->ReleaseMesh(Asset);
		break;
	case EAssetType::Texture:
		mTextureManager->ReleaseTexture(Asset);
		break;
	case EAssetType::Material:
		mMaterialManager->ReleaseMaterial(Asset);
		break;
	case EAssetType::Animation2D:
		mAnimation2DManager->ReleaseAnimation(Asset);
		break;
	case EAssetType::Sound:
		mSoundManager->ReleaseSound(Asset);
		break;
	}
}
