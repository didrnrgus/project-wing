#pragma once

#include "../GameInfo.h"

struct FUIBrush
{
	CSharedPtr<class CTexture>	Texture;
	FVector4D	Tint = FVector4D::White;
	std::vector<FAnimationFrame>	Frames;
	bool	AnimationEnable = false;
	int		Frame = 0;
	float	Time = 0.f;
	float	FrameTime = 0.f;
	float	PlayTime = 1.f;
	float	PlayRate = 1.f;
};

namespace SlotType
{
	enum class Type
	{
		ToAdd = 0, // +모양
		Added = 1, // 빈칸 -> 내부에 다른 이미지 나올것임.
	};
}

namespace ItemType
{
	enum class Type
	{
		Heal = 0,
		Dex = 1,
		Ghost = 2
	};
}