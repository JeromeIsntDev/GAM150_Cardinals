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
	: _sound{}, _group{}, _volume(0.f), _pitch(0.f), _loop(0)
{
	PRINT("Audio Created\n");
}

void Audio::Load_Sound(AudioFile audio_file)
{
	_sound = AEAudioLoadSound(audio_file.filename);
}

void Audio::Load_Music(AudioFile audio_file)
{
	_sound = AEAudioLoadMusic(audio_file.filename);
}


void Audio::Play_Audio() const
{
	PRINT("Play Audio Check\n");
	AEAudioPlay(_sound, _group, _volume, _pitch, _loop);
}

void Audio::Play_Audio(f32 volume, f32 pitch, s32 loop)
{
	if (_group.fmod_group == NULL) {
		PRINT("MISSING GROUP");
	}
	PRINT("Play Audio Check with values set\n");
	_volume = volume;
	_pitch = pitch;
	_loop = loop;
	AEAudioPlay(_sound, _group, volume, pitch, loop);
}

void Audio::Free_Audio() const
{
	AEAudioUnloadAudio(_sound);
}

void AudioGroup::Create_Audio_Group() {

}

void AudioGroup::Pause_Audio_Group(AEAudioGroup _group) {

}

void AudioGroup::Resume_Audio_Group(AEAudioGroup _group) {

}

void AudioGroup::Stop_Audio_Group(AEAudioGroup _group) {

}

void AudioGroup::Free_Audio_Group(AEAudioGroup audio_gp){

}