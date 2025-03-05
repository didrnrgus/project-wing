#pragma once

#include "Share/Object.h"

class CAsset abstract :
    public CObject
{
public:
    CAsset();
    virtual ~CAsset();

protected:
    class CScene* mScene = nullptr;
    std::string     mName;
    EAssetType      mAssetType;

public:
    const std::string& GetName()    const
    {
        return mName;
    }

    EAssetType GetAssetType()   const
    {
        return mAssetType;
    }

public:
    void SetName(const std::string& Name)
    {
        mName = Name;
    }
};

