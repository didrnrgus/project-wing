#pragma once

#include "../GameInfo.h"
#include "Animation2DSequence.h"
#include "Shader/Animation2DCBuffer.h"

class CAnimation2D
{
	friend class CSpriteComponent;

private:
	static CAnimation2DCBuffer* mAnimCBuffer;

public:
	static void CreateCBuffer();
	static void DestroyCBuffer();
	static void DisableAnimation();

private:
	CAnimation2D();
	CAnimation2D(const CAnimation2D& Anim);
	virtual ~CAnimation2D();

private:
	class CScene* mScene = nullptr;
	class CSpriteComponent* mOwner = nullptr;
	std::unordered_map<std::string, CAnimation2DSequence*>	mSequenceMap;
	CAnimation2DSequence* mCurrentSequence = nullptr;

public:
	bool Init();
	void Update(float DeltaTime);
	bool AddSequence(const std::string& Name,
		float PlayTime = 1.f, float PlayRate = 1.f,
		bool Loop = false, bool Reverse = false);
	bool AddSequence(class CAnimation2DData* Asset,
		float PlayTime = 1.f, float PlayRate = 1.f,
		bool Loop = false, bool Reverse = false);
	void SetPlayTime(const std::string& Name,
		float PlayTime);
	void SetPlayRate(const std::string& Name,
		float PlayRate);
	void SetLoop(const std::string& Name,
		bool Loop);
	void SetReverse(const std::string& Name,
		bool Reverse);
	void ChangeAnimation(const std::string& Name);
	CAnimation2D* Clone();
	void SetShader();
	
private:
	CAnimation2DSequence* FindSequence(
		const std::string& Name);

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void(T::* Func)())
	{
		CAnimation2DSequence* Sequence = FindSequence(Name);

		if (!Sequence)
			return;

		Sequence->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name,
		int Frame, T* Obj, void(T::* Func)())
	{
		CAnimation2DSequence* Sequence = FindSequence(Name);

		if (!Sequence)
			return;

		Sequence->AddNotify<T>(Name, Frame, Obj, Func);
	}
};

