/*!***********************************************************************
\file	AudioManager.cpp
\author	Shirley Liau
\date	16/01/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	AudioManager is responsible for music and sound effect.

\par	Changes: None
**************************************************************************/

#include "../header/AudioManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

AudioManager::AudioManager()
	: musicGroup{nullptr}, sfxGroup{nullptr}, masterVol{ 1.0f }, musicVol{ 1.0f }, sfxVol{ 1.0f }
{
	PRINT("Audio Created\n");
}

AudioManager::~AudioManager()
{
	PRINT("Audio Destroyed\n");
}

void AudioManager::Init()
{
	musicGroup = AEAudioCreateGroup();
	sfxGroup = AEAudioCreateGroup();
	PRINT("Audio System Initialized\n");
}

void AudioManager::Update()
{
	
}

void AudioManager::LoadAudio(const std::string& name, const std::string& path)
{
	if (audioMap.find(name) != audioMap.end()) return;
	AEAudio sound = AEAudioLoadSound(path.c_str());

	if (sound.fmod_sound)
	{
		audioMap[name] = sound;
		PRINT("Audio Loaded: %s from %s\n", name.c_str(), path.c_str());
	}
	else
	{
		PRINT("Failed to load audio: %s\n", path.c_str());
	}
}

void AudioManager::PlayAudio(const std::string& name, AudioType type, f32 pitch, s32 loop)
{	
	auto it = audioMap.find(name);
	if (it == audioMap.end()) 
	{
		PRINT("Audio not found: %s\n", name.c_str());
		return;
	}

	// Choose group based on type (Enum)
	AEAudioGroup targetGroup = (type == AudioType::MUSIC) ?  musicGroup : sfxGroup;
	AEAudioPlay(it->second, targetGroup, 1.0f, pitch, loop);
}

void AudioManager::SetVolume(AudioType type, f32 vol)
{
	// Keep volume between 0% and 100%
	if (vol < 0.0f) vol = 0.0f;
	if (vol > 1.0f) vol = 1.0f;

	// Set volume based on type
	switch (type)
	{
		case AudioType::MASTER: masterVol = vol; break;
		case AudioType::MUSIC: musicVol = vol; break;
		case AudioType::SFX: sfxVol = vol; break;
	}

	// Update group volumes
	AEAudioSetGroupVolume(musicGroup, masterVol * musicVol);
	AEAudioSetGroupVolume(sfxGroup, masterVol * sfxVol);
}

void AudioManager::PauseAudio()
{
	AEAudioPauseGroup(musicGroup);
	AEAudioPauseGroup(sfxGroup);
}

void AudioManager::ResumeAudio()
{
	AEAudioResumeGroup(musicGroup);
	AEAudioResumeGroup(sfxGroup);
}

void AudioManager::StopAudio()
{
	AEAudioStopGroup(musicGroup);
	AEAudioStopGroup(sfxGroup);
}

void AudioManager::Free()
{
	for(auto const& pair : audioMap) 
	{
		AEAudioUnloadAudio(pair.second);
	}

	audioMap.clear();
	PRINT("Audio System Freed\n");
}

void AudioManager::LoadManifest(const std::string& path)
{
	std::ifstream file(path);
	std::string line;

	if (!file.is_open())
	{
		PRINT("ERROR: where tf is ur manifest?? at %s\n", path.c_str());
		return;

	while (std::getline(file, line))
	{
		// skip empty or lines starting with #
		if (line.empty() || line[0] == '#') continue;

		std::stringstream ss(line);
		std::string name, filePath;

		// parses name and path ts just like Prasanna Ghali
		if (std::getline(ss, name, ',') && std::getline(ss, filePath))
		{
			filePath.erase(0, filePath.find_first_not_of(" \t"));
			LoadAudio(name, filePath);
		}
	}
}