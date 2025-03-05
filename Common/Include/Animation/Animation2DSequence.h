#pragma once

#include "../GameInfo.h"

struct FAnimation2DNotify
{
	std::string	Name;
	int		Frame = 0;
	std::function<void()>	Function;
};

class CAnimation2DSequence
{
	friend class CAnimation2D;

private:
	CAnimation2DSequence();
	CAnimation2DSequence(const CAnimation2DSequence& Anim);
	~CAnimation2DSequence();

private:
	class CAnimation2D* mOwner = nullptr;
	CSharedPtr<class CAnimation2DData>	mAsset;
	// 여러 프레임 중 몇번째 프레임인지를 판단.
	int		mFrame = 0;
	// 프레임을 증가 혹은 감소 시키기 위해 누적되는 시간
	float	mTime = 0.f;
	// 1프레임이 변경되어야 하는 시간
	float	mFrameTime = 0.f;
	// 예를 들어 7개의 프레임일 경우 7개가 한바퀴 돌아서
	// 동작되는 시간
	float	mPlayTime = 1.f;
	// 재생속도 배율
	float	mPlayRate = 1.f;
	// 이 애니메이션이 계속 반복해서 재생되어야 하는지
	// 아니면 한번 반복하고 마지막 프레임에 멈춰 있어야
	// 하는지를 나타낸다.
	bool	mLoop = false;
	// 역재생인지 아닌지.
	bool	mReverse = false;
	// 이 모션이 1번 재생을 완료할때마다 호출되는 함수
	bool					mEndFunctionEnable = true;
	std::function<void()>	mEndFunction;
	std::vector<FAnimation2DNotify*>	mNotifyList;

public:
	CAnimation2DData* GetAnimationAsset()	const
	{
		return mAsset;
	}

	int GetFrame()	const
	{
		return mFrame;
	}

	float GetTime()	const
	{
		return mTime;
	}

	const std::string& GetName()	const;

public:
	void SetAsset(class CAnimation2DData* Asset);
	void SetPlayTime(float PlayTime);

	void SetPlayRate(float Rate)
	{
		mPlayRate = Rate;
	}

	void SetLoop(bool Loop)
	{
		mLoop = Loop;
	}

	void SetReverse(bool Reverse)
	{
		mReverse = Reverse;
	}

public:
	void Update(float DeltaTime);
	CAnimation2DSequence* Clone();

private:
	void CallNotify();

public:
	template <typename T>
	void SetEndFunction(T* Obj, void(T::* Func)())
	{
		mEndFunction = std::bind(Func, Obj);
	}

	template <typename T>
	void AddNotify(const std::string& Name,
		int Frame, T* Obj, void(T::* Func)())
	{
		FAnimation2DNotify* Notify = new FAnimation2DNotify;

		Notify->Name = Name;
		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		mNotifyList.emplace_back(Notify);
	}
};

