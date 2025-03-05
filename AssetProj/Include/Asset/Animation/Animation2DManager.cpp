#include "Animation2DManager.h"
#include "Animation2DData.h"
#include "Animation/Animation2D.h"

CAnimation2DManager::CAnimation2DManager()
{
}

CAnimation2DManager::~CAnimation2DManager()
{
    CAnimation2D::DestroyCBuffer();
}

bool CAnimation2DManager::Init()
{
    CAnimation2D::CreateCBuffer();

    CreateAnimation("PlayerIdle");
    SetAnimationTextureType("PlayerIdle",
        EAnimationTextureType::SpriteSheet);
    SetTexture("PlayerIdle", "PlayerSprite",
        TEXT("Texture\\Player\\Player.png"));

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            AddFrame("PlayerIdle", j * 50.f, 185.f + i * 37.f, 
                50.f, 37.f);
        }
    }

    CreateAnimation("PlayerRun");
    SetAnimationTextureType("PlayerRun",
        EAnimationTextureType::SpriteSheet);
    SetTexture("PlayerRun", "PlayerSprite");
    // 2, 8
    for (int i = 0; i < 5; ++i)
    {
        AddFrame("PlayerRun", (2 + i) * 50.f, 8 * 37.f,
            50.f, 37.f);
    }

    CreateAnimation("PlayerWalk");
    SetAnimationTextureType("PlayerWalk",
        EAnimationTextureType::Frame);

    std::vector<const TCHAR*>   FileNames;
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-walk-00.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-walk-01.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-walk-02.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-walk-03.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-walk-04.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-walk-05.png"));
    SetTexture("PlayerWalk", "PlayerWalk",
        FileNames);
    AddFrameCount("PlayerWalk", 6, 0.f, 0.f, 1.f, 1.f);

    // adventurer-punch-00, 6개
    CreateAnimation("PlayerAttack");
    SetAnimationTextureType("PlayerAttack",
        EAnimationTextureType::Frame);

    FileNames.clear();
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-punch-00.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-punch-01.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-punch-02.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-punch-03.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-punch-04.png"));
    FileNames.emplace_back(TEXT("Texture\\Player\\PlayerFrame\\adventurer-punch-05.png"));
    SetTexture("PlayerAttack", "PlayerAttack",
        FileNames);
    AddFrameCount("PlayerAttack", 6, 0.f, 0.f, 1.f, 1.f);


    CreateAnimation("GunnerIdle");
    SetAnimationTextureType("GunnerIdle",
        EAnimationTextureType::SpriteSheet);
    SetTexture("GunnerIdle", "GunnerSprite",
        TEXT("Texture\\Monster.png"));
    
    for (int i = 0; i < 14; ++i)
    {
        AddFrame("GunnerIdle", i * 45.f, 60.f,
            45.f, 60.f);
    }

    CreateAnimation("GunnerAttack");
    SetAnimationTextureType("GunnerAttack",
        EAnimationTextureType::SpriteSheet);
    SetTexture("GunnerAttack", "GunnerSprite");

    for (int i = 0; i < 21; ++i)
    {
        AddFrame("GunnerAttack", i * 45.f, 180.f,
            45.f, 60.f);
    }

    CreateAnimation("GunnerSkill");
    SetAnimationTextureType("GunnerSkill",
        EAnimationTextureType::SpriteSheet);
    SetTexture("GunnerSkill", "GunnerSprite");

    for (int i = 0; i < 5; ++i)
    {
        AddFrame("GunnerSkill", i * 45.f, 120.f,
            45.f, 60.f);
    }

    CreateAnimation("Explosion");
    SetAnimationTextureType("Explosion",
        EAnimationTextureType::Frame);

    FileNames.clear();

    TCHAR   Path[89][MAX_PATH] = {};

    for (int i = 1; i <= 89; ++i)
    {
        wsprintf(Path[i - 1], TEXT("Texture\\Explosion\\Explosion%d.png"), i);
        FileNames.emplace_back(Path[i - 1]);
    }

    SetTexture("Explosion", "Explosion",
        FileNames);
    AddFrameCount("Explosion", 89, 0.f, 0.f, 1.f, 1.f);

    return true;
}

bool CAnimation2DManager::CreateAnimation(
    const std::string& Name)
{
    if (FindAnimation(Name))
        return false;

    CAnimation2DData* Animation = new CAnimation2DData;

    Animation->SetName(Name);

    mAnimationMap.insert(std::make_pair(Name, Animation));

    return true;
}

bool CAnimation2DManager::SetAnimationTextureType(
    const std::string& Name, EAnimationTextureType Type)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetAnimationTextureType(Type);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name, 
    CTexture* Texture)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(Texture);

    return true;
}

bool CAnimation2DManager::SetTexture(const std::string& Name, 
    const std::string& TextureName)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name, 
    const std::string& TextureName, const TCHAR* FileName)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName, FileName);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name,
    const std::string& TextureName,
    std::vector<const TCHAR*> FileName)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName, FileName);

    return true;
}

bool CAnimation2DManager::SetTexture(
    const std::string& Name, 
    const std::string& TextureName, const TCHAR* FileName,
    const TCHAR* Ext, int Count)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTexture(TextureName, FileName,
        Ext, Count);

    return true;
}

bool CAnimation2DManager::SetTextureFullPath(
    const std::string& Name, 
    const std::string& TextureName, const TCHAR* FullPath)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTextureFullPath(TextureName, FullPath);

    return true;
}

bool CAnimation2DManager::SetTextureFullPath(
    const std::string& Name, 
    const std::string& TextureName,
    std::vector<const TCHAR*> FullPath)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->SetTextureFullPath(TextureName, FullPath);

    return true;
}

bool CAnimation2DManager::AddFrame(const std::string& Name,
    const FVector2D& Start, const FVector2D& Size)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrame(Start, Size);

    return true;
}

bool CAnimation2DManager::AddFrame(const std::string& Name,
    float StartX, float StartY, float SizeX, float SizeY)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrame(StartX, StartY, SizeX, SizeY);

    return true;
}

bool CAnimation2DManager::AddFrameCount(
    const std::string& Name, int Count, 
    const FVector2D& Start, const FVector2D& Size)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrameCount(Count, Start, Size);

    return true;
}

bool CAnimation2DManager::AddFrameCount(
    const std::string& Name, int Count,
    float StartX, float StartY, float SizeX, float SizeY)
{
    CAnimation2DData* Animation = FindAnimation(Name);

    if (!Animation)
        return false;

    Animation->AddFrameCount(Count,
        StartX, StartY, SizeX, SizeY);

    return true;
}

CAnimation2DData* CAnimation2DManager::FindAnimation(
    const std::string& Name)
{
    auto    iter = mAnimationMap.find(Name);

    if (iter == mAnimationMap.end())
        return nullptr;

    return iter->second;
}

void CAnimation2DManager::ReleaseAnimation(
    CAsset* Animation)
{
    auto    iter = mAnimationMap.find(Animation->GetName());

    if (iter != mAnimationMap.end())
    {
        // 다른곳에서 가지고 있는게 없기 때문에 제거한다.
        if (iter->second->GetRefCount() == 1)
        {
            mAnimationMap.erase(iter);
        }
    }
}
