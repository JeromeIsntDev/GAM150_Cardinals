/*!***********************************************************************
\file	CameraEffects.cpp
\author	Zoey Tan
\date	25/03/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	CameraEffects is responsible for managing camera effects in the game.

\par	Changes: None
**************************************************************************/

#include "../header/CameraEffects.hpp"

void CameraEffects::Init(Player::GravityDirection startGrav)
{
	offsetX = 0.f;
	offsetY = 0.f;
	shakeTimeLeft = 0.f;
	shakeMagnitude = 0.f;
	springX = 0.f;
	springVelX = 0.f;
	springY = 0.f;
	springVelY = 0.f;
	prevGravDir = startGrav;
	wasObstacleHitLastFrame = false;
	AEGfxSetCamPosition(0.f, 0.f);
}

void CameraEffects::Update(f32 dt, Player::GravityDirection currentGrav, bool obstacleHit)
{
	// reset offset
	offsetX = 0.f;
	offsetY = 0.f;

	// spring effect
	if (currentGrav != prevGravDir)
	{
		switch (currentGrav)
		{
		case Player::GravityDirection::Up:
			springVelY -= springImpulse;
			break;
		case Player::GravityDirection::Down:
			springVelY += springImpulse;
			break;
		case Player::GravityDirection::Left:
			springVelX -= springImpulse;
			break;
		case Player::GravityDirection::Right:
			springVelX += springImpulse;
		default:
			break;
		}
		prevGravDir = currentGrav;
	}

	springVelX += (-springStrength * springX - springDamping * springVelX) * dt;
	springVelY += (-springStrength * springY - springDamping * springVelY) * dt;

	springX += springVelX * dt;
	springY += springVelY * dt;

	if (fabsf(springX) < 0.05f) springX = 0.f;
	if (fabsf(springY) < 0.05f) springY = 0.f;
	if (fabsf(springVelX) < 0.05f) springVelX = 0.f;
	if (fabsf(springVelY) < 0.05f) springVelY = 0.f;

	offsetX += springX;
	offsetY += springY;

	// shake effect
	if (obstacleHit && !wasObstacleHitLastFrame)
	{
		shakeTimeLeft = defaultShakeDuration;
		shakeMagnitude = defaultShakeMagnitude;
	}
	wasObstacleHitLastFrame = obstacleHit;

	if (shakeTimeLeft > 0.f)
	{
		shakeTimeLeft -= dt;

		offsetX += (((rand() % 100) / 50.f) - 1.f) * shakeMagnitude;
		offsetY += (((rand() % 100) / 50.f) - 1.f) * shakeMagnitude;

		if (shakeTimeLeft < 0.f)
		{
			shakeTimeLeft = 0.f;
		}
	}
}

void CameraEffects::Apply() const
{
	f32 snappedX = roundf(offsetX);
	f32 snappedY = roundf(offsetY);
	AEGfxSetCamPosition(snappedX, snappedY);
}

void CameraEffects::Clear()
{
	offsetX = 0.f;
	offsetY = 0.f;
	shakeTimeLeft = 0.f;
	shakeMagnitude = 0.f;
	springX = 0.f;
	springVelX = 0.f;
	springY = 0.f;
	springVelY = 0.f;
	wasObstacleHitLastFrame = false;
	prevGravDir = Player::GravityDirection::Down;
	AEGfxSetCamPosition(0.f, 0.f);
}

AEVec2 CameraEffects::GetOffset() const
{
	return { offsetX, offsetY };
}