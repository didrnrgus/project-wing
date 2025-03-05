#pragma once

#include "GameInfo.h"

class CMaterialManager
{
	friend class CAssetManager;

private:
	CMaterialManager();
	~CMaterialManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CMaterial>>	mMaterialMap;

public:
	bool Init();

	bool CreateMaterial(const std::string& Name);

	class CMaterial* FindMaterial(const std::string& Name);

	void ReleaseMaterial(class CAsset* Material);
};

