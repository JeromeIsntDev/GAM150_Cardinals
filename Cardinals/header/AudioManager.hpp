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
    constexpr const AudioFile BGM{ "../../Assets/bouken.mp3" };

    //Sound effect
    constexpr const AudioFile Effect{ "../../Assets/ore.mp3" };
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

    void CreateAudioGroup();

    void Load_Audio(AudioFile audio);
    void Play_Audio() const;
    void Play_Audio(s32 _loop);
    void Free_Audio() const;

    //Group
    void Pause_Audio_Group();
    void Resume_Audio_Group();
    void Stop_Audio_Group();
    void Set_Audio_Volume(f32 volume);
    void Set_Audio_Pitch(f32 pitch);
    void Free_Audio_Group();
};

#endif // !_Texture_Man_Header_