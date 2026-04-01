/**
 * @file CameraEffects.hpp
 * @brief Header file for the CameraEffects class
 * @details This class manages camera effects such as shaking and parallax background movement 
 *			based on the player's gravity direction 
 * 			and obstacle interactions.
 * @author Zoey Tan 
 * @date 25.03.2026
 */

#pragma once
#ifndef _Camera_Effects_Header_
#define _Camera_Effects_Header_

#include "PlayerController.hpp"
#include <cmath>

/**
 * @class CameraEffects
 * @brief Manages camera effects such as shaking and parallax background movement based on player interactions.
 * 
 */
class CameraEffects {
private:
	f32 offsetX{ 0.f };
	f32 offsetY{ 0.f };

	f32 shakeTimeLeft{ 0.f };
	f32 shakeMagnitude{ 0.f };
	f32 defaultShakeMagnitude{ 1.7f };
	f32 defaultShakeDuration{ 0.15f };

	f32 gravBgOffsetX{ 0.f };
	f32 gravBgOffsetY{ 0.f };
	f32 gravBgVelX{ 0.f };
	f32 gravBgVelY{ 0.f };
	Player::GravityDirection prevParallaxGrav{ Player::GravityDirection::Down };
	bool wasObstacleHitLastFrame{ false };

public:
	/**
	 * @brief Initializes the camera effects
	 * 
	 * @param startGrav 
	 */
	void Init(Player::GravityDirection startGrav);

	/**
	 * @brief Updates the camera effects
	 * 
	 * @param dt 
	 * @param currentGrav 
	 * @param obstacleHit 
	 */
	void Update(f32 dt, Player::GravityDirection currentGrav, bool obstacleHit);

	/**
	 * @brief Applies the camera effects
	 * 
	 */
	void Apply() const;

	/**
	 * @brief Clears the camera effects
	 * 
	 */
	void Clear();

	/**
	 * @brief Get the Offset object
	 * 
	 * @return AEVec2 
	 */
	AEVec2 GetOffset() const;

	/**
	 * @brief Get the Bg Offset object
	 * 
	 * @return AEVec2 
	 */
	AEVec2 GetBgOffset() const;
};

#endif
