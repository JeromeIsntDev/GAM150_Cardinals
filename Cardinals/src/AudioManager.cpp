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

Audio::Audio() 
	: _musicGroup{}, _sfxGroup{}, _masterVol{ 1.0f }, _musicVol{ 1.0f }, _sfxVol{ 1.0f }
{
	PRINT("Audio Created\n");
}

Audio::~Audio()
{
	PRINT("Audio Destroyed\n");
}

//void Audio::SetAudioGroup(AEAudioGroup group)
//{
//	_group = group;
//}

void Audio::Init_Audio_System()
{
	_musicGroup = AEAudioCreateGroup();
	_sfxGroup = AEAudioCreateGroup();
	PRINT("Audio System Initialized\n");
}

void Audio::Update_Audio_System()
{
	
}

void Audio::Load_Audio(const std::string& name, AudioFile audio_file)
{
	// Check if audio already loaded
	if (_audioMap.find(name) != _audioMap.end()) return;
	AEAudio sound = AEAudioLoadSound(audio_file.filename);
	_audioMap[name] = sound;
	PRINT("Audio Loaded: %s\n", name.c_str());
}

void Audio::Play_Audio(const std::string& name, AudioType type, f32 pitch, s32 loop)
{	
	if (_audioMap.find(name) == _audioMap.end()) {
		PRINT("Audio not found: %s\n", name.c_str());
		return;
	}
	// Choose group based on type (Enum)
	AEAudioGroup targetGroup;
	if(type == AudioType::MUSIC) {
		targetGroup = _musicGroup;
	}
	else {
		targetGroup = _sfxGroup;
	}
	AEAudioPlay(_audioMap[name], targetGroup, 1.0f, pitch, loop);
}

void Audio::Set_Volume(AudioType type, f32 vol)
{
	// Keep volume between 0% and 100%
	if (vol < 0.0f) vol = 0.0f;
	if (vol > 1.0f) vol = 1.0f;

	// Set volume based on type
	switch (type)
	{
		case AudioType::MASTER:
			_masterVol = vol;
			break;
		case AudioType::MUSIC:
			_musicVol = vol;
			break;
		case AudioType::SFX:
			_sfxVol = vol;
			break;
	}
	// Update group volumes
	AEAudioSetGroupVolume(_musicGroup, _masterVol * _musicVol);
	AEAudioSetGroupVolume(_sfxGroup, _masterVol * _sfxVol);

}

void Audio::Pause_Audio()
{
	AEAudioPauseGroup(_musicGroup);
	AEAudioPauseGroup(_sfxGroup);
}

void Audio::Resume_Audio()
{
	AEAudioResumeGroup(_musicGroup);
	AEAudioResumeGroup(_sfxGroup);
}

void Audio::Stop_Audio()
{
	AEAudioStopGroup(_musicGroup);
	AEAudioStopGroup(_sfxGroup);
}

void Audio::Free_Audio_System()
{
	for(auto const& pair : _audioMap) {
		AEAudioUnloadAudio(pair.second);
	}
	_audioMap.clear();
	PRINT("Audio System Freed\n");
}

//void Audio::Pause_Audio_Group() const {
//	AEAudioPauseGroup(_group);
//}
//
//void Audio::Resume_Audio_Group() const {
//	AEAudioResumeGroup(_group);
//}
//
//void Audio::Stop_Audio_Group() const {
//	AEAudioStopGroup(_group);
//}
//
//void Audio::Set_Audio_Volume(f32 volume) {
//	_volume = volume;
//	AEAudioSetGroupVolume(_group, volume);
//}
//
//void Audio::Set_Audio_Pitch(f32 pitch) {
//	_pitch = pitch;
//	AEAudioSetGroupPitch(_group, pitch);
//}
//
//void Audio::Free_Audio_Group() const{
//	AEAudioUnloadAudioGroup(_group);
//}
