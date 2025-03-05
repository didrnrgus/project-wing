#pragma once

#include "../Asset.h"

class CAnimation2DData :
    public CAsset
{
    friend class CAnimation2DSequence;

public:
    CAnimation2DData();
    virtual ~CAnimation2DData();

private:
    CSharedPtr<class CTexture>      mTexture;
    EAnimationTextureType           mType = EAnimationTextureType::SpriteSheet;
    std::vector<FAnimationFrame>    mFrameList;

public:
    EAnimationTextureType GetAnimationTextureType() const
    {
        return mType;
    }

    class CTexture* GetTexture()    const
    {
        return mTexture;
    }

    const FAnimationFrame& GetFrame(int Index)  const
    {
        return mFrameList[Index];
    }

    int GetFrameCount() const
    {
        return (int)mFrameList.size();
    }

public:
    void SetAnimationTextureType(EAnimationTextureType Type)
    {
        mType = Type;
    }

    void SetTexture(class CTexture* Texture);
    void SetTexture(const std::string& TextureName);
    void SetTexture(const std::string& Name,
        const TCHAR* FileName);
    void SetTexture(const std::string& Name,
        std::vector<const TCHAR*> FileName);
    void SetTexture(const std::string& Name,
        const TCHAR* FileName, const TCHAR* Ext,
        int Count);
    void SetTextureFullPath(const std::string& Name,
        const TCHAR* FullPath);
    void SetTextureFullPath(const std::string& Name,
        std::vector<const TCHAR*> FullPath);
    void AddFrame(const FVector2D& Start,
        const FVector2D& Size);
    void AddFrame(float StartX, float StartY,
        float SizeX, float SizeY);
    void AddFrameCount(int Count, const FVector2D& Start,
        const FVector2D& Size);
    void AddFrameCount(int Count, float StartX, float StartY,
        float SizeX, float SizeY);
};

