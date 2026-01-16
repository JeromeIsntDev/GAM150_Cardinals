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


AEAudio Audio::Load_Sound(AudioFile audio_file)
{
	AEAudio audio = AEAudioLoadSound(audio_file.filename);
	return audio;
}

AEAudio Audio::Load_Music(AudioFile audio_file)
{
	AEAudio audio = AEAudioLoadMusic(audio_file.filename);
	return audio;
}

AEAudioGroup Audio::Create_Audio_Group()
{
	AEAudioGroup audio_gp = AEAudioCreateGroup();
	AEAudioSetGroupVolume(audio_gp, 0.5f); //set default volume 50%
	AEAudioSetGroupPitch(audio_gp, 0.5f); //set default volume 50%
	return audio_gp;
}

void Audio::Play_Audio(AEAudio audio, AEAudioGroup group, f32 volume, f32 pitch, s32 loop)
{
	AEAudioPlay(audio, group, volume, pitch, loop);
}

void Audio::Pause_Audio_Group(AEAudioGroup audio_gp)
{
	AEAudioPauseGroup(audio_gp);
}

void Audio::Resume_Audio_Group(AEAudioGroup audio_gp)
{
	AEAudioResumeGroup(audio_gp);
}

void Audio::Stop_Audio_Group(AEAudioGroup audio_gp)
{
	AEAudioStopGroup(audio_gp);
}

void Audio::Free_Audio(AEAudio audio)
{
	AEAudioUnloadAudio(audio);
}

void Audio::Free_Audio_Group(AEAudioGroup audio_gp)
{
	AEAudioUnloadAudioGroup(audio_gp);
}
