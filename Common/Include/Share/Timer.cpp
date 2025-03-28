#include "Timer.h"

LARGE_INTEGER CTimer::mSecond = {};
LARGE_INTEGER CTimer::mTime = {};
float CTimer::mDeltaTime = 0.f;

float CTimer::mFPS = 0.f;
float CTimer::mFPSTime = 0.f;
int CTimer::mFPSTick = 0;

void CTimer::Init()
{
	QueryPerformanceFrequency(&mSecond);
	QueryPerformanceCounter(&mTime);
}

float CTimer::Update()
{
	LARGE_INTEGER	Time;

	QueryPerformanceCounter(&Time);

	mDeltaTime = (Time.QuadPart - mTime.QuadPart) / (float)mSecond.QuadPart;

	mTime = Time;

	mFPSTime += mDeltaTime;
	++mFPSTick;

	if (mFPSTick == 60)
	{
		mFPS = mFPSTick / mFPSTime;
		mFPSTick = 0;
		mFPSTime = 0.f;
	}

	return mDeltaTime;
}

wchar_t* CTimer::GetFPS()
{
	wchar_t	FPSText[64] = {};
	swprintf_s(FPSText, L"FPS : %.2f", mFPS);
	return FPSText;
}

float CTimer::GetDeltaTime()
{
	return mDeltaTime;
}
