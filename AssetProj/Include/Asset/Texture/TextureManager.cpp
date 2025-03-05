#include "TextureManager.h"
#include "Texture.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

bool CTextureManager::Init()
{
    return true;
}

bool CTextureManager::LoadTexture(const std::string& Name,
    const TCHAR* FileName)
{
    CTexture* Texture = FindTexture(Name);

    if (Texture)
        return true;

    Texture = new CTexture;

    Texture->SetName(Name);

    if (!Texture->LoadTexture(FileName))
    {
        SAFE_DELETE(Texture);
        return false;
    }

    mTextureMap.insert(std::make_pair(Name, Texture));

    return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& Name,
    const TCHAR* FullPath)
{
    CTexture* Texture = FindTexture(Name);

    if (Texture)
        return true;

    Texture = new CTexture;

    Texture->SetName(Name);

    if (!Texture->LoadTextureFullPath(FullPath))
    {
        SAFE_DELETE(Texture);
        return false;
    }

    mTextureMap.insert(std::make_pair(Name, Texture));

    return true;
}

bool CTextureManager::LoadTexture(const std::string& Name,
    const std::vector<const TCHAR*>& FileName)
{
    CTexture* Texture = FindTexture(Name);

    if (Texture)
        return true;

    Texture = new CTexture;

    Texture->SetName(Name);

    if (!Texture->LoadTexture(FileName))
    {
        SAFE_DELETE(Texture);
        return false;
    }

    mTextureMap.insert(std::make_pair(Name, Texture));

    return true;
}

bool CTextureManager::LoadTextureFullPath(
    const std::string& Name, 
    const std::vector<const TCHAR*>& FullPath)
{
    CTexture* Texture = FindTexture(Name);

    if (Texture)
        return true;

    Texture = new CTexture;

    Texture->SetName(Name);

    if (!Texture->LoadTextureFullPath(FullPath))
    {
        SAFE_DELETE(Texture);
        return false;
    }

    mTextureMap.insert(std::make_pair(Name, Texture));

    return true;
}

bool CTextureManager::LoadTexture(const std::string& Name,
    const TCHAR* FileName, const TCHAR* Ext, int Count)
{
    CTexture* Texture = FindTexture(Name);

    if (Texture)
        return true;

    Texture = new CTexture;

    Texture->SetName(Name);

    if (!Texture->LoadTexture(FileName, Ext, Count))
    {
        SAFE_DELETE(Texture);
        return false;
    }

    mTextureMap.insert(std::make_pair(Name, Texture));

    return true;
}

CTexture* CTextureManager::FindTexture(
    const std::string& Name)
{
    auto    iter = mTextureMap.find(Name);

    if (iter == mTextureMap.end())
        return nullptr;

    return iter->second;
}

void CTextureManager::ReleaseTexture(CAsset* Texture)
{
    auto    iter = mTextureMap.find(Texture->GetName());

    if (iter != mTextureMap.end())
    {
        // 다른곳에서 가지고 있는게 없기 때문에 제거한다.
        if (iter->second->GetRefCount() == 1)
        {
            mTextureMap.erase(iter);
        }
    }
}
