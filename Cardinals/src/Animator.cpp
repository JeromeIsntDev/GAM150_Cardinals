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

namespace {
	Texture playerTexture[3]{};
}

void Animator::SetAnimArr() {
	playerTexture[0].Load_Image(Images::Player);
	playerTexture[1].Load_Image(Images::PlayerJump);
	playerTexture[2].Load_Image(Images::Player);
	std::copy(std::begin(playerTexture), std::end(playerTexture), animArr.begin());
	currenTexture = animArr[0];
}

void Animator::AnimationUpd(f32 const& dt) {
	if (!isPlaying)return;
	//functionality here
	animTime += dt;
	if (animTime >= frameDur) {
		animTime -= frameDur;

		if (isLoop) {
			currFrame = (currFrame + 1) % ANIM_FRAMES;
		}
		else {
			if (currFrame < ANIM_FRAMES - 1) {
				currFrame++;
			}
			else {
				currFrame = ANIM_FRAMES - 1; // stay at last frame
				isPlaying = false;          // optional: stop animation
			}
		}
	}
	currenTexture = animArr[currFrame];
}

void Animator::UnloadAnimFrames() {
	for (Texture T : playerTexture) {
		T.Unload_Image();
	}
}

void Animator::SetFrameTime(f32 time) {
	frameDur = time;
}

void Animator::SetLoop(bool value) {
	isLoop = value;
}

void Animator::SetPlay(bool value) {
	isPlaying = value;
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

Texture& Animator::GetCurrTexture(){
	return currenTexture;
}

Texture& Animator::GetIdleTexture() {
	return animArr[0];
}

std::array<Texture, Animator::ANIM_FRAMES> Animator::GetArr() const {
	return animArr;
}