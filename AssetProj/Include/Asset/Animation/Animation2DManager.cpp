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

	//기본 헬기 이미지
	{
		CreateAnimation("PlayerIdle");
		SetAnimationTextureType("PlayerIdle", EAnimationTextureType::SpriteSheet);
		SetTexture("PlayerIdle", "PlayerSprite", TEXT("Texture\\player_12x128.png")); // 423 * 600

		int sizeX = 128;
		int sizeY = 128;

		for (int i = 0; i < 12; ++i)
		{
			AddFrame("PlayerIdle", sizeX * i, sizeY, sizeX, sizeY);
		}
	}

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
