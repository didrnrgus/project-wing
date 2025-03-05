#pragma once

#include "Sound.h"

class CSoundManager
{
	friend class CAssetManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	FMOD::System* mSystem = nullptr;
	FMOD::ChannelGroup* mMasterGroup = nullptr;
	std::unordered_map<std::string, FMOD::ChannelGroup*>	mChannelGroupMap;
	std::unordered_map<std::string, CSharedPtr<CSound>>		mSoundMap;

public:
	bool Init();
	void Update();
	bool CreateChannelGroup(const std::string& Name);
	bool LoadSound(const std::string& Name, const std::string& GroupName,
		bool Loop, const char* FileName);
	bool LoadSoundFullPath(const std::string& Name, const std::string& GroupName,
		bool Loop, const char* FullPath);

	void Play(const std::string& Name);
	void Stop(const std::string& Name);
	void Pause(const std::string& Name);
	void Resume(const std::string& Name);
	// Volume : 0 ~ 100
	void SetMasterVolume(int Volume);
	void SetGroupVolume(const std::string& GroupName, int Volume);

private:
	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);

public:
	CSound* FindSound(const std::string& Name);

	void ReleaseSound(class CAsset* Sound);
};

