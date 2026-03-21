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

struct AudioFile { const char* filename; };

namespace Audios
{
    constexpr const AudioFile BGM{ "../../Assets/Music/bouken.mp3" };
    constexpr const AudioFile SFX{ "../../Assets/Music/ore.mp3" };
    constexpr const AudioFile CLICK{ "../../Assets/Music/btn_click.wav" };
    constexpr const AudioFile WALK{ "../../Assets/Music/walk.wav" };
    constexpr const AudioFile JUMP{ "../../Assets/Music/jump.wav" };
    constexpr const AudioFile HIT{ "../../Assets/Music/obstacle_hit.wav" };
    constexpr const AudioFile HITWIN{ "../../Assets/Music/wingate_hit.wav" };
    constexpr const AudioFile PICKUP{ "../../Assets/Music/pick_up.wav" };
    constexpr const AudioFile NEXT{ "../../Assets/Music/next.wav" };
    constexpr const AudioFile FAIL_EFFECT{ "../../Assets/Music/next.wav" };
    constexpr const AudioFile GRAVITY{ "../../Assets/Music/gravity_change.wav" };
}

enum class AudioType { SFX, MUSIC, MASTER };

class AudioManager
{
private:

    AudioManager();
    ~AudioManager();

    std::map<std::string, AEAudio> audioMap;

    // Two audio groups for music and sfx
    AEAudioGroup musicGroup, sfxGroup;

    f32 masterVol = 1.0f, musicVol = 1.0f, sfxVol = 1.0f;

public:

    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }

    AudioManager(const AudioManager&) = delete;
    void operator=(const AudioManager&) = delete;

    void Init();
	void Update();
	void Free(); // exit

    void LoadAudio(const std::string& name, const std::string& path);
    void PlayAudio(const std::string& name, AudioType type, f32 pitch = 1.0f, s32 loop = 0);
	void SetVolume(AudioType type, f32 vol);

    //Group
    void PauseAudio() const;
    void ResumeAudio() const;
    void StopAudio() const;

    // text reading for asset loading lol
    void LoadManifest(const std::string& path);
};

#endif // !_Texture_Man_Header_