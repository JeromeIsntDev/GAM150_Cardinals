/*!***********************************************************************
\file	AudioManager.cpp
\author	Shirley Liau
\date	16/01/2026
\par	Last Updated by: Shirley Liau
\par	Project: Cardinals

\brief	AudioManager is responsible for music and sound effect.

\par	Changes: None
**************************************************************************/

#include "../header/AudioManager.hpp"


Audio::Audio() 
	: _sound{}, _group{}, _volume(0.5f), _pitch(0.5f), _loop(0)
{
	PRINT("Audio Created\n");
}

void Audio::SetAudioGroup(AEAudioGroup group)
{
	_group = group;
}

void Audio::Load_Audio(AudioFile audio_file)
{
	_sound = AEAudioLoadSound(audio_file.filename);
}

void Audio::Play_Audio() const
{	
	AEAudioPlay(_sound, _group, _volume, _pitch, _loop);
}

void Audio::Play_Audio(s32 loop)
{
	_loop = loop;
	AEAudioPlay(_sound, _group, _volume, _pitch, loop);
}

void Audio::Free_Audio() const
{
	AEAudioUnloadAudio(_sound);
}

void Audio::Pause_Audio_Group() const {
	AEAudioPauseGroup(_group);
}

void Audio::Resume_Audio_Group() const {
	AEAudioResumeGroup(_group);
}

void Audio::Stop_Audio_Group() const {
	AEAudioStopGroup(_group);
}

void Audio::Set_Audio_Volume(f32 volume) {
	_volume = volume;
	AEAudioSetGroupVolume(_group, volume);
}

void Audio::Set_Audio_Pitch(f32 pitch) {
	_pitch = pitch;
	AEAudioSetGroupPitch(_group, pitch);
}

void Audio::Free_Audio_Group() const{
	AEAudioUnloadAudioGroup(_group);
}