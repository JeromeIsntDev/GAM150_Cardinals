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

Animator::Animator():
	animTime{0}, animArr{}
{}

void Animator::SetAnimTime(f32 time) {
	animTime = time;
}

f32 Animator::GetAnimTime() const {
	return animTime;
}

std::array<TileUV, Animator::ANIM_FRAMES> Animator::GetArr() const {
	return animArr;
}