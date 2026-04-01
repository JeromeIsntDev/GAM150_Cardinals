/**
 * @file		AudioManager.hpp
 * @brief		Header for managing audios, sound effects, and volume.
 * @details		Handles reading from an AudioManifest and the general controlling of audios.
 * @author		Shirley Liau, Chua Jia Xuan Breanne, Zoey Tan
 * @date		20.01.2026
 */

#pragma once
#ifndef _Audio_Man_Header_
#define _Audio_Man_Header_

#include <string>
#include <map>
#include <AEEngine.h>
#include <iostream>
#include <fstream>
#include <sstream>

 /**
  * @brief Simple container for audio file paths
  * @param filename The constant character pointer to the asset's location
  */
struct AudioFile { const char* filename; };

/**
 * @brief Categorization for audio routing and volume control
 * SFX: Short sound effects
 * MUSIC: Background tracks
 * MASTER: Global control affecting both SFX and MUSIC
 */
enum class AudioType { SFX, MUSIC, MASTER };

/**
 * @brief AudioManager class to handle background music and sound effects
 * * This singleton manages the loading, playback, and volume control of audio
 * assets using designated audio groups for Music and SFX.
 */
class AudioManager
{
private:

    /**
     * @brief Private Constructor for Singleton pattern
     * Initializes internal audio groups and volume defaults.
     */
    AudioManager();

    /**
     * @brief Destructor to ensure clean shutdown
     * Frees loaded audio assets and destroys audio groups.
     */
    ~AudioManager();

    /**
     * @brief Map containing all loaded AEAudio handles keyed by name
     */
    std::map<std::string, AEAudio> audioMap;

    /**
     * @brief Alpha Engine audio groups for categorized playback control
     */
    AEAudioGroup musicGroup, sfxGroup;

    /**
     * @brief Stored volume levels for the different audio categories
     * @param masterVol Global volume multiplier (0.0f - 1.0f)
     * @param musicVol Specific multiplier for the music group
     * @param sfxVol Specific multiplier for the sound effects group
     */
    f32 masterVol = 1.0f, musicVol = 1.0f, sfxVol = 1.0f;

public:

    /**
     * @brief Access the Singleton instance of the AudioManager
     * @return AudioManager& Reference to the static instance
     */
    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }

    /**
     * @brief Deleted copy constructor
     * * Prevents the AudioManager from being copied to ensure only one
     * instance exists at all times.
     */
    AudioManager(const AudioManager&) = delete;

    /**
     * @brief Deleted assignment operator
     * Prevents the AudioManager from being assigned to another object,
     * maintaining the integrity of the Singleton instance.
     */
    void operator=(const AudioManager&) = delete;

    /**
     * @brief Initializes the audio system and creates audio groups
     * @return void
     */
    void Init();

    /**
     * @brief Frees all loaded audio resources and shuts down the audio system
     * @return void
     */
    void Free();

    /**
     * @brief Loads a single audio file into the managed audio map
     * @param name The unique string identifier for the audio
     * @param path The file system path to the audio asset
     * @return void
     */
    void LoadAudio(const std::string& name, const std::string& path);\

    /**
     * @brief Plays a specific audio by name with optional pitch and looping
     * @param name The identifier of the audio to play
     * @param type The category (SFX, MUSIC, MASTER) for groups
     * @param pitch The playback speed/pitch multiplier
     * @param loop Number of times to loop
     * @return void
     */
    void PlayAudio(const std::string& name, AudioType type, f32 pitch = 1.0f, s32 loop = 0);

    /**
     * @brief Updates the volume for a specific audio category
     * @param type The audio group to modify
     * @param vol Normalized volume level (0.0f to 1.0f)
     * @return void
     */
	void SetVolume(AudioType type, f32 vol);

    /**
     * @brief Retrieves the current volume level of a specific category
     * @param type The audio group to query
     * @return f32 The current normalized volume level
     */
	f32 GetVolume(AudioType type) const;

    /**
     * @brief Pauses all active audio groups
     * @return void
     */
    void PauseAudio() const;

    /**
     * @brief Resumes all currently paused audio groups
     * * @return void
     */
    void ResumeAudio() const;

    /**
     * @brief Stops all playback across all audio groups
     * @return void
     */
    void StopAudio() const;

    /**
     * @brief Parses an external file to bulk-load audio assets
     * @param path The path to (specifically) the audio manifest file
     * @return void
     */
    void LoadManifest(const std::string& path);
};

#endif // !_Texture_Man_Header_