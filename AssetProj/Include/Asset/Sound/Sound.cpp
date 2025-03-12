#include "Sound.h"

#ifdef _DEBUG
#pragma comment(lib, "../Bin/Debug/fmod_vc.lib")
#else
#pragma comment(lib, "../Bin/Release/fmod_vc.lib")
#endif // _DEBUG

CSound::CSound()
{
}

CSound::~CSound()
{
	if (mSound)
		mSound->release();
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group,
	bool Loop, const char* FileName)
{
	mSystem = System;
	mGroup = Group;
	mLoop = Loop;

	char	FullPath[MAX_PATH] = {};

	strcpy_s(FullPath, gRootPathMultibyte);
	strcat_s(FullPath, "..\\Asset\\");
	strcat_s(FullPath, FileName);

	FMOD_MODE	Mode = FMOD_DEFAULT;

	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	if (mSystem->createSound(FullPath, Mode, nullptr, &mSound) != FMOD_OK)
		return false;

	return true;
}

bool CSound::LoadSoundFullPath(FMOD::System* System, 
	FMOD::ChannelGroup* Group, bool Loop, const char* FullPath)
{
	mSystem = System;
	mGroup = Group;
	mLoop = Loop;

	FMOD_MODE	Mode = FMOD_DEFAULT;

	if (Loop)
		Mode = FMOD_LOOP_NORMAL;

	if (mSystem->createSound(FullPath, Mode, nullptr, &mSound) != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	// 마지막 인자의 채널에 이 사운드를 재생시키는 채널을 얻어온다.
	// 마지막 채널은 이 사운드를 겹쳐셔 몇번을 재생하더라도 1개의 채널그룹을
	// 이용하여 재생을 하게 된다.
	// 따라서 Stop을 위해서는 몇번 겹쳐서 재생되고 있는지를 저장해야 한다.
	mSystem->playSound(mSound, mGroup, false, &mChannel);
	++mPlayCount;

	mChannel->setCallback(CSound::SoundEnd);
}

void CSound::Stop()
{
	if (mChannel)
	{
		--mPlayCount;
		bool	Playing = false;

		mChannel->isPlaying(&Playing);

		if (Playing)
		{
			mChannel->stop();

			if (mPlayCount == 0)
				mChannel = nullptr;
		}
	}
}

void CSound::Pause()
{
	if (mChannel)
	{
		bool	Playing = false;

		mChannel->isPlaying(&Playing);

		if (Playing)
			mChannel->setPaused(true);
	}
}

void CSound::Resume()
{
	if (mChannel)
	{
		bool	Pause = false;

		mChannel->getPaused(&Pause);

		if (Pause)
			mChannel->setPaused(false);
	}
}

FMOD_RESULT CSound::SoundEnd(FMOD_CHANNELCONTROL* ChannelControl, 
	FMOD_CHANNELCONTROL_TYPE Type, 
	FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
	void* commanddata1, void* commanddata2)
{
	CLog::PrintLog("SoundEnd");

	return FMOD_OK;
}
