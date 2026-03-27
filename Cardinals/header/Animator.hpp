/*!***********************************************************************
\file	Animator.hpp
\author	Sadric Koh
\date	10/01/26
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief
holds the class Animator that will take in the images for animation to loop

\par	Changes:
**************************************************************************/

#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP
#pragma once
#include "../header/SpritesheetHandler.hpp"
#include "../header/TextureManager.hpp"
#include <array>
class Animator : SpriteSheet {
public:
	enum { ANIM_FRAMES = 6 };
	Animator():
		isLoop{ true }, isPlaying{ false }, 
		animTime{0.f}, frameDur{0.f},
		currFrame{0},
		animArr{}
	{}
	void SetFrameTime(f32);
	void AnimationUpd(f32&);
public:
	/*
		CONSTANT FUNCTIONS
	*/
	int GetCurrFrame()const;
	f32 GetFrameDur() const;
	f32 GetAnimTime()const;
	bool GetLoop() const;
	bool GetPlaying() const;
	std::array<int, ANIM_FRAMES> GetArr() const;
private:
	bool isLoop;
	bool isPlaying;
	f32 animTime;
	f32 frameDur; //how long to hold the frame
	int currFrame;
	std::array<int, ANIM_FRAMES> animArr;
};

#endif // !

