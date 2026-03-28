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
#include <algorithm> 
#include <iostream>
class Animator : SpriteSheet {
public:
	enum { ANIM_FRAMES = 3 };
	Animator():
		isLoop{ false }, isPlaying{ false }, 
		animTime{0.f}, frameDur{0.f},
		currFrame{ 0 }, currenTexture{},
		animArr{}
	{}
	~Animator() {
		std::for_each(animArr.begin(), animArr.end(), [](Texture& T) {
			T.Unload_Image();
		});
		//UnloadAnimFrames();
	}
	void SetFrameTime(f32);
	void AnimationUpd(f32 const&);
public:
	/*
		CONSTANT FUNCTIONS
	*/
	int GetCurrFrame()const;
	f32 GetFrameDur() const;
	f32 GetAnimTime()const;
	bool GetLoop() const;
	bool GetPlaying() const;
	Texture& GetCurrTexture();
	Texture& GetIdleTexture();
	std::array<Texture, ANIM_FRAMES> GetArr() const;
	void SetAnimArr();
	void SetLoop(bool);
	void SetPlay(bool);
	void UnloadAnimFrames();
private:
	bool isLoop;
	bool isPlaying;
	f32 animTime;
	f32 frameDur; //how long to hold the frame
	int currFrame;
	Texture currenTexture;
	std::array<Texture, ANIM_FRAMES> animArr;
};

#endif // !

