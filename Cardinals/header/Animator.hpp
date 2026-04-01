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

/**
 * @class Animator inherited from spritesheet
 * @brief Holds the functions and variables for a simple animator to function
 */
class Animator : SpriteSheet {
public:
	enum { ANIM_FRAMES = 3 };
	/**
	 * @brief Default Ctor for the animator
	 */
	Animator():
		isLoop{ false }, isPlaying{ false }, 
		animTime{0.f}, frameDur{0.f},
		currFrame{ 0 }, currenTexture{},
		animArr{}
	{}
	/**
	 * @brief Unloads all the frame images for the animator
	 */
	void UnloadFrames();
	/**
	 * @brief Give a frame Time to set for how long each frame should be
	 *
	 * @param f32 value to be set
	 */
	void SetFrameTime(f32);
	/**
	 * @brief The update function for the animator
	 *
	 * @param f32 const reference of deltatime
	 */
	void AnimationUpd(f32 const&);
public:
	/*
		CONSTANT FUNCTIONS
	*/

	/**
	 * @brief get the current frame in int value
	 *
	 * @return the int value of the frame
	 */
	int GetCurrFrame()const;
	/**
	 * @brief Gets the current duration time of the animation
	 *
	 * @return the duration of animation
	 */
	f32 GetFrameDur() const;
	/**
	 * @brief Gets the animation time
	 *
	 * @return Returns the animation time
	 */
	f32 GetAnimTime()const;
	/**
	 * @brief Gets the boolean if an animation is looping or not
	 *
	 * @return Boolean of animation looping
	 */
	bool GetLoop() const;
	/**
	 * @brief Gets the boolean if an animation is playing or not
	 *
	 * @return Boolean of animation playing
	 */
	bool GetPlaying() const;
	/**
	 * @brief Gets the current texture within the animation
	 *
	 * @return Texture reference of the current texture
	 */
	Texture& GetCurrTexture();
	/**
	 * @brief Gets the idle texture[0] of the animation
	 *
	 * @return Texture reference of the idle texture
	 */
	Texture& GetIdleTexture();
	/**
	 * @brief Gets the array of textures for the animation
	 *
	 * @return the array for the animation
	 */
	std::array<Texture, ANIM_FRAMES> GetArr() const;
	/**
	 * @brief Set the array with textures for the animation
	 *
	 */
	void SetAnimArr();
	/**
	 * @brief Set the loop to be on or off
	 *
	 * @param boolean value to be parsed in to turn on looping
	 */
	void SetLoop(bool);
	/**
	 * @brief Set play to be on or off
	 *
	 * @param boolean value to be parsed in to turn on animation
	 */
	void SetPlay(bool);
	//void UnloadAnimFrames();
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

