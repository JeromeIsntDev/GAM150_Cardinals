/*!***********************************************************************
\file	AudioManager.hpp
\author	Shirley Liau
\date	16/01/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	AudioManager is responsible for music and sound effect.

\par	Changes: None
**************************************************************************/
#pragma once
#ifndef _Audio_Man_Header_
#define _Audio_Man_Header_
#include <string>
#include <map>
#include <AEEngine.h>

struct AudioFile {
    const char* filename;
};

namespace Audios
{
    //Music file
    constexpr const AudioFile BGM{ "../../Assets/Music/bouken.mp3" };

    //Sound effect
    constexpr const AudioFile SFX{ "../../Assets/Music/ore.mp3" };
}

enum class AudioType {
    SFX,
    MUSIC,
	MASTER
};

class Audio
{
private:
    Audio();
    ~Audio();
    std::map < std::string, AEAudio> _audioMap;
    // Two audio groups for music and sfx
    AEAudioGroup _musicGroup;
	AEAudioGroup _sfxGroup;
    //f32 _volume;
    //f32 _pitch;
    //s32 _loop;
    f32 _masterVol = 1.0f;
	f32 _musicVol = 1.0f;
	f32 _sfxVol = 1.0f;

public:
    //Constructor Overload
    static Audio& Instance()
    {
        static Audio instance;
        return instance;
	}

    void Init_Audio_System();
	void Update_Audio_System();
	void Free_Audio_System();

    void Load_Audio(const std::string&name, AudioFile audio_file);
    void Play_Audio(const std::string& name, AudioType type, f32 pitch = 1.0f, s32 loop = 0);
	void Set_Volume(AudioType type, f32 vol);

    //Group
    void Pause_Audio();
    void Resume_Audio();
    void Stop_Audio();
    //void SetAudioGroup(AEAudioGroup group);
    //void Load_Audio(AudioFile audio);
    //void Play_Audio() const;
    //void Play_Audio(s32 _loop);
    //void Free_Audio() const;

    //Group
    //void Pause_Audio_Group() const;
    //void Resume_Audio_Group() const;
    //void Stop_Audio_Group() const;
    //void Set_Audio_Volume(f32 volume);
    //void Set_Audio_Pitch(f32 pitch);
    //void Free_Audio_Group() const;
};



#endif // !_Texture_Man_Header_