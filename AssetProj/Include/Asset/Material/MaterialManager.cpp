#include "MaterialManager.h"
#include "Material.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
    CMaterial::DestroySampler();
}

bool CMaterialManager::Init()
{
    CMaterial::SetSampler(ETextureSamplerType::Point);
    CMaterial::SetSampler(ETextureSamplerType::Linear);
    CMaterial::SetSampler(ETextureSamplerType::Anisotropic);

    CreateMaterial("DefaultMaterial");

    CMaterial* Mtrl = FindMaterial("DefaultMaterial");

    Mtrl->SetPixelShader("DefaultMaterialShader");
    Mtrl->SetSamplerType(ETextureSamplerType::Linear);

	return true;
}

bool CMaterialManager::CreateMaterial(
	const std::string& Name)
{
    if (FindMaterial(Name))
        return false;

    CMaterial* Material = new CMaterial;

    Material->SetName(Name);
    Material->SetPixelShader("DefaultMaterialShader");

    mMaterialMap.insert(std::make_pair(Name, Material));

	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& Name)
{
    auto    iter = mMaterialMap.find(Name);

    if (iter == mMaterialMap.end())
        return nullptr;

    return iter->second;
}

void CMaterialManager::ReleaseMaterial(CAsset* Material)
{
    auto    iter = mMaterialMap.find(Material->GetName());

    if (iter != mMaterialMap.end())
    {
        // 다른곳에서 가지고 있는게 없기 때문에 제거한다.
        if (iter->second->GetRefCount() == 1)
        {
            mMaterialMap.erase(iter);
        }
    }
}
