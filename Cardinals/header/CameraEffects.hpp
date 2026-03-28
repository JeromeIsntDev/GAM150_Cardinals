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

class CameraEffects {
private:
	f32 offsetX{ 0.f };
	f32 offsetY{ 0.f };

	f32 shakeTimeLeft{ 0.f };
	f32 shakeMagnitude{ 0.f };
	f32 defaultShakeMagnitude{ 1.7f };
	f32 defaultShakeDuration{ 0.2f };

	f32 springX{ 0.f };
	f32 springVelX{ 0.f };
	f32 springY{ 0.f };
	f32 springVelY{ 0.f };
	f32 springStrength{ 40.f };
	f32 springDamping{ 20.f };
	f32 springImpulse{ 60.f };

	bool wasObstacleHitLastFrame{ false };
	Player::GravityDirection prevGravDir{ Player::GravityDirection::Down };

public:
	void Init(Player::GravityDirection startGrav);
	void Update(f32 dt, Player::GravityDirection currentGrav, bool obstacleHit);
	void Apply() const;
	void Clear();

	AEVec2 GetOffset() const;
};

#endif
