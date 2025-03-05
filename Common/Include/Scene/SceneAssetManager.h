#pragma once

#include "../GameInfo.h"

class CSceneAssetManager
{
	friend class CScene;

private:
	CSceneAssetManager();
	~CSceneAssetManager();

private:
	class CScene* mScene = nullptr;

private:
	std::unordered_map<std::string, CSharedPtr<class CAsset>>	mAssetMap;

public:
	bool Init();

#pragma region Mesh

public:
	bool CreateMesh(const std::string& Name, void* VertexData, int Size, int Count,
		D3D11_USAGE VertexUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive,
		void* IndexData = nullptr, int IndexSize = 0, int IndexCount = 0,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	class CMesh* FindMesh(const std::string& Name);

#pragma endregion Mesh


#pragma region Texture

public:
	bool LoadTexture(const std::string& Name,
		const TCHAR* FileName);
	bool LoadTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name,
		const std::vector<const TCHAR*>& FileName);
	bool LoadTextureFullPath(const std::string& Name,
		const std::vector<const TCHAR*>& FullPath);
	bool LoadTexture(const std::string& Name,
		const TCHAR* FileName, const TCHAR* Ext,
		int Count);

	class CTexture* FindTexture(const std::string& Name);

#pragma endregion Texture

#pragma region Material

	bool CreateMaterial(const std::string& Name);

	class CMaterial* FindMaterial(const std::string& Name);

#pragma endregion Material

#pragma region Animation

    bool CreateAnimation(const std::string& Name);

    bool SetAnimationTextureType(const std::string& Name,
        EAnimationTextureType Type);
    bool SetAnimationTexture(const std::string& Name,
        class CTexture* Texture);
    bool SetAnimationTexture(const std::string& Name,
        const std::string& TextureName,
        const TCHAR* FileName);
    bool SetAnimationTexture(const std::string& Name,
        const std::string& TextureName,
        std::vector<const TCHAR*> FileName);
    bool SetAnimationTexture(const std::string& Name,
        const std::string& TextureName,
        const TCHAR* FileName, const TCHAR* Ext,
        int Count);
    bool SetAnimationTextureFullPath(const std::string& Name,
        const std::string& TextureName,
        const TCHAR* FullPath);
    bool SetAnimationTextureFullPath(const std::string& Name,
        const std::string& TextureName,
        std::vector<const TCHAR*> FullPath);
    bool AddAnimationFrame(const std::string& Name,
        const FVector2D& Start,
        const FVector2D& Size);
    bool AddAnimationFrame(const std::string& Name,
        float StartX, float StartY,
        float SizeX, float SizeY);
    bool AddAnimationFrameCount(const std::string& Name,
        int Count, const FVector2D& Start,
        const FVector2D& Size);
    bool AddAnimationFrameCount(const std::string& Name,
        int Count, float StartX, float StartY,
        float SizeX, float SizeY);

    class CAnimation2DData* FindAnimation(const std::string& Name);

#pragma endregion Animation

#pragma region Sound

    bool LoadSound(const std::string& Name, const std::string& GroupName,
        bool Loop, const char* FileName);
    bool LoadSoundFullPath(const std::string& Name, const std::string& GroupName,
        bool Loop, const char* FullPath);

    class CSound* FindSound(const std::string& Name);

#pragma endregion Sound


#pragma region Font

    bool LoadFont(const std::string& Name, const TCHAR* FontName,
        int Weight, float FontSize, const TCHAR* LocalName,
        int Stretch = DWRITE_FONT_STRETCH_NORMAL);
    bool LoadFontCollection(const std::string& Name,
        const TCHAR* FileName);
    const TCHAR* GetFontFaceName(const std::string& CollectionName);
    bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b,
        unsigned char a);
    bool CreateFontColor(const FVector4D& Color);

    class CFont* FindFont(const std::string& Name);
    class CFontCollection* FindFontCollection(const std::string& Name);
    ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g,
        unsigned char b, unsigned char a);
    ID2D1SolidColorBrush* FindFontColor(const FVector4D& Color);

#pragma endregion Font
};

