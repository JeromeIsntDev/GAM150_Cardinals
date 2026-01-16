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

#include <AEAudio.h>

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
public:

    AEAudio Load_Sound(AudioFile audio);
    AEAudio Load_Music(AudioFile audio);
    AEAudioGroup Create_Audio_Group();
    void Play_Audio(AEAudio audio, AEAudioGroup group, f32 volume, f32 pitch, s32 loop);
    void Pause_Audio_Group(AEAudioGroup group);
    void Resume_Audio_Group(AEAudioGroup group);
    void Stop_Audio_Group(AEAudioGroup group);
    void Free_Audio(AEAudio audio);
    void Free_Audio_Group(AEAudioGroup audio_gp);
};

#endif // !_Texture_Man_Header_