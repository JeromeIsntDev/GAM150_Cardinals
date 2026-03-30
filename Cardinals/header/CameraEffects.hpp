/*!***********************************************************************
\file	CameraEffects.hpp
\author	Zoey Tan
\date	25/03/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	CameraEffects is responsible for managing camera effects in the game.

\par	Changes: None
**************************************************************************/

#pragma once
#ifndef _Camera_Effects_Header_
#define _Camera_Effects_Header_

#include "PlayerController.hpp"
#include <cmath>

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
	void Init(Player::GravityDirection startGrav);
	void Update(f32 dt, Player::GravityDirection currentGrav, bool obstacleHit);
	void Apply() const;
	void Clear();

	AEVec2 GetOffset() const;
	AEVec2 GetBgOffset() const;
};

#endif
