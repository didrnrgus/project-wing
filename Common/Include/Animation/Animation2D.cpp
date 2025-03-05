#include "Animation2D.h"
#include "Asset/Animation/Animation2DData.h"
#include "Asset/Animation/Animation2DManager.h"
#include "Asset/AssetManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneAssetManager.h"
#include "../Component/SpriteComponent.h"
#include "Asset/Texture/Texture.h"

CAnimation2DCBuffer* CAnimation2D::mAnimCBuffer = nullptr;

void CAnimation2D::CreateCBuffer()
{
	mAnimCBuffer = new CAnimation2DCBuffer;

	mAnimCBuffer->Init();
}

void CAnimation2D::DestroyCBuffer()
{
	SAFE_DELETE(mAnimCBuffer);
}

void CAnimation2D::DisableAnimation()
{
	mAnimCBuffer->SetAnimation2DEnable(false);

	mAnimCBuffer->UpdateBuffer();
}

CAnimation2D::CAnimation2D()
{
}

CAnimation2D::CAnimation2D(const CAnimation2D& Anim)
{
	*this = Anim;
}

CAnimation2D::~CAnimation2D()
{
	auto	iter = mSequenceMap.begin();
	auto	iterEnd = mSequenceMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Update(float DeltaTime)
{
	mCurrentSequence->Update(DeltaTime);
}

bool CAnimation2D::AddSequence(const std::string& Name, 
	float PlayTime, float PlayRate, bool Loop, bool Reverse)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (Sequence)
		return false;

	CAnimation2DData* Data = nullptr;

	if (mScene)
		Data = mScene->GetAssetManager()->FindAnimation(Name);

	else
		Data = CAssetManager::GetInst()->GetAnimationManager()->FindAnimation(Name);

	if (!Data)
		return false;

	Sequence = new CAnimation2DSequence;

	Sequence->mOwner = this;

	Sequence->SetAsset(Data);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(Loop);
	Sequence->SetReverse(Reverse);

	// 처음 지정된 Sequence로 현재 Sequence를 지정해둔다.
	if (!mCurrentSequence)
	{
		mCurrentSequence = Sequence;

		mOwner->SetTexture(
			mCurrentSequence->GetAnimationAsset()->GetTexture());
	}

	mSequenceMap.insert(std::make_pair(Name, Sequence));

	return true;
}

bool CAnimation2D::AddSequence(CAnimation2DData* Asset,
	float PlayTime, float PlayRate, bool Loop, bool Reverse)
{
	CAnimation2DSequence* Sequence = FindSequence(Asset->GetName());

	if (Sequence)
		return false;

	Sequence = new CAnimation2DSequence;

	Sequence->mOwner = this;

	Sequence->SetAsset(Asset);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(Loop);
	Sequence->SetReverse(Reverse);

	// 처음 지정된 Sequence로 현재 Sequence를 지정해둔다.
	if (!mCurrentSequence)
	{
		mCurrentSequence = Sequence;
		mOwner->SetTexture(
			mCurrentSequence->GetAnimationAsset()->GetTexture());
	}

	mSequenceMap.insert(std::make_pair(Asset->GetName(),
		Sequence));

	return true;
}

void CAnimation2D::SetPlayTime(const std::string& Name,
	float PlayTime)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->SetPlayTime(PlayTime);
}

void CAnimation2D::SetPlayRate(const std::string& Name,
	float PlayRate)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->SetPlayRate(PlayRate);
}

void CAnimation2D::SetLoop(const std::string& Name,
	bool Loop)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->SetLoop(Loop);
}

void CAnimation2D::SetReverse(const std::string& Name,
	bool Reverse)
{
	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->SetReverse(Reverse);
}

void CAnimation2D::ChangeAnimation(const std::string& Name)
{
	if (Name.empty())
		return;

	else if (!mCurrentSequence)
		return;

	else if (mCurrentSequence->GetName() == Name)
		return;

	mCurrentSequence->mFrame = 0;
	mCurrentSequence->mTime = 0.f;
	mCurrentSequence->mEndFunctionEnable = true;

	CAnimation2DSequence* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	mCurrentSequence = Sequence;

	mCurrentSequence->mFrame = 0;
	mCurrentSequence->mTime = 0.f;

	mOwner->SetTexture(
		mCurrentSequence->GetAnimationAsset()->GetTexture());
}

CAnimation2D* CAnimation2D::Clone()
{
	return new CAnimation2D(*this);
}

void CAnimation2D::SetShader()
{
	// 애니메이션 정보를 Shader에 넘겨준다.
	float LTX = 0.f, LTY = 0.f, RBX = 1.f, RBY = 1.f;

	EAnimationTextureType	TexType =
		mCurrentSequence->mAsset->GetAnimationTextureType();

	CTexture* Texture = mCurrentSequence->mAsset->GetTexture();

	const FAnimationFrame& Frame = 
		mCurrentSequence->mAsset->GetFrame(mCurrentSequence->mFrame);

	switch (TexType)
	{
	case EAnimationTextureType::SpriteSheet:
		LTX = Frame.Start.x / Texture->GetTexture()->Width;
		LTY = Frame.Start.y / Texture->GetTexture()->Height;
		RBX = LTX + Frame.Size.x / Texture->GetTexture()->Width;
		RBY = LTY + Frame.Size.y / Texture->GetTexture()->Height;
		mOwner->SetTextureIndex(0);
		break;
	case EAnimationTextureType::Frame:
		mOwner->SetTextureIndex(mCurrentSequence->mFrame);
		break;
	}

	mAnimCBuffer->SetAnimation2DEnable(true);
	mAnimCBuffer->SetUV(LTX, LTY, RBX, RBY);

	mAnimCBuffer->UpdateBuffer();
}

CAnimation2DSequence* CAnimation2D::FindSequence(
	const std::string& Name)
{
	auto	iter = mSequenceMap.find(Name);

	if (iter == mSequenceMap.end())
		return nullptr;

	return iter->second;
}
