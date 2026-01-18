/*!***********************************************************************
\file	AudioManager.hpp
\author	Shirley Liau
\date	16/01/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	AudioManager is responsible for music and sound effect.

\par	Changes: None
**************************************************************************/
#pragma once
#ifndef _Audio_Man_Header_
#define _Audio_Man_Header_

#include <AEEngine.h>

struct AudioFile {
    const char* filename;
};

namespace Audios
{
    //Music file
    constexpr const AudioFile BGM{ "../../Assets/PlanetTexture.png" };

    //Sound effect
    constexpr const AudioFile Effect{ "../../Assets/PlanetTexture.png" };
}


class Audio
{
private:
    AEAudio _sound;
    AEAudioGroup _group;
    f32 _volume;
    f32 _pitch;
    s32 _loop;

public:
    //Constructor Overload
    Audio();

    void Load_Sound(AudioFile audio);
    void Load_Music(AudioFile audio);
    void Play_Audio() const;
    void Play_Audio(f32 _volume, f32 _pitch, s32 _loop);
    void Free_Audio() const;
    
};

class AudioGroup {
private:
    f32 _volume;
    f32 _pitch;

public:
    void Create_Audio_Group();
    void Pause_Audio_Group(AEAudioGroup _group);
    void Resume_Audio_Group(AEAudioGroup _group);
    void Stop_Audio_Group(AEAudioGroup _group);
    void Free_Audio_Group(AEAudioGroup audio_gp);

};

#endif // !_Texture_Man_Header_