/*!***********************************************************************
\file	Animator.cpp
\author	Sadric Koh
\date	10/01/26
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief
Functions definition for Animator objects

\par	Changes:
**************************************************************************/

#include "../header/Animator.hpp"

void Animator::AnimationUpd(f32& dt) {
	if (!isPlaying)return;
	//functionality here
	animTime += dt;
	if (animTime >= frameDur) {
		animTime = 0.f;
		//go to next frame
		currFrame++;
	}
}

void Animator::SetFrameTime(f32 time) {
	frameDur = time;
}

bool Animator::GetLoop() const {
	return isLoop;
}
bool Animator::GetPlaying() const {
	return isPlaying;
}

int Animator::GetCurrFrame() const {
	return currFrame;
}

f32 Animator::GetFrameDur() const {
	return frameDur;
}
f32 Animator::GetAnimTime() const {
	return animTime;
}

std::array<TileUV, Animator::ANIM_FRAMES> Animator::GetArr() const {
	return animArr;
}