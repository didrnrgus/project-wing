#pragma once

#include "../Asset.h"
#include "fmod.hpp"

class CSound :
    public CAsset
{
    friend class CSoundManager;

protected:
    CSound();
    virtual ~CSound();

protected:
    FMOD::System* mSystem = nullptr;
    FMOD::ChannelGroup* mGroup = nullptr;
    // 이 사운드가 어떤 채널에서 재생되고 있는지를 저장해둔다.
    FMOD::Channel* mChannel = nullptr;
    FMOD::Sound* mSound = nullptr;

    bool        mLoop = false;
    int         mPlayCount = 0;

public:
    bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group,
        bool Loop, const char* FileName);
    bool LoadSoundFullPath(FMOD::System* System, FMOD::ChannelGroup* Group,
        bool Loop, const char* FullPath);
    void Play();
    void Stop();
    void Pause();
    void Resume();

public:
    // 사운드 재생이 종료될 경우 호출될 콜백함수.
    static FMOD_RESULT SoundEnd(FMOD_CHANNELCONTROL* ChannelControl,
        FMOD_CHANNELCONTROL_TYPE Type,
        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
        void* commanddata1, void* commanddata2);
};

