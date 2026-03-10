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
#include <array>
class Animator : SpriteSheet {
public:
	enum { ANIM_FRAMES = 6 };
	Animator() = default;
	void SetAnimTime(f32);
public:
	/*
		CONSTANT FUNCTIONS
	*/
	f32 GetAnimTime()const;
	std::array<TileUV, ANIM_FRAMES> GetArr() const;
private:
	f32 animTime;
	std::array<TileUV, ANIM_FRAMES> animArr;
};

#endif // !

