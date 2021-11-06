#pragma once
#include "Common.h"

class Sound
{
public:
    Sound() = default;
    ~Sound();
    bool Load(const string & FileName,float Volume = 1.0f );
    bool LoadMusic(const string& FileName);
    void Play()const;
    void PlayMusic()const;
    string GetName()const;

private:
    Mix_Chunk* m_pSoundData = nullptr;
    Mix_Music* m_pMusic = nullptr;
    string m_FileName;
};

