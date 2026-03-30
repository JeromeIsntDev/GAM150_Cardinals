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
	
	gravBgOffsetX = 0.f;
	gravBgOffsetY = 0.f;
	gravBgVelX = 0.f;
	gravBgVelY = 0.f;

	prevParallaxGrav = startGrav;


	wasObstacleHitLastFrame = false;
	AEGfxSetCamPosition(0.f, 0.f);
}

void CameraEffects::Update(f32 dt, Player::GravityDirection currentGrav, bool obstacleHit)
{
	// reset offset
	offsetX = 0.f;
	offsetY = 0.f;

	// parallax effect
	if (currentGrav != prevParallaxGrav)
	{
		switch (currentGrav)
		{
			case Player::GravityDirection::Up:
				gravBgVelY = -110.f;
				break;
			case Player::GravityDirection::Down:
				gravBgVelY = 110.f;
				break;
			case Player::GravityDirection::Left:		
				gravBgVelX = -60.f;
				break;
			case Player::GravityDirection::Right:
				gravBgVelX = 60.f;
				break;
			default:
				break;
		}
		prevParallaxGrav = currentGrav;
	}

	gravBgVelX += (-25.f * gravBgOffsetX - 13.f * gravBgVelX) * dt;
	gravBgVelY += (-25.f * gravBgOffsetY - 13.f * gravBgVelY) * dt;

	gravBgOffsetX += gravBgVelX * dt;
	gravBgOffsetY += gravBgVelY * dt;

	if (fabsf(gravBgOffsetX) < 0.05f) gravBgOffsetX = 0.f;
	if (fabsf(gravBgOffsetY) < 0.05f) gravBgOffsetY = 0.f;
	if (fabsf(gravBgVelX) < 0.05f) gravBgVelX = 0.f;
	if (fabsf(gravBgVelY) < 0.05f) gravBgVelY = 0.f;

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
	
	gravBgOffsetX = 0.f;
	gravBgOffsetY = 0.f;
	gravBgVelX = 0.f;
	gravBgVelY = 0.f;

	prevParallaxGrav = Player::GravityDirection::Down;

	wasObstacleHitLastFrame = false;
	AEGfxSetCamPosition(0.f, 0.f);
}

AEVec2 CameraEffects::GetOffset() const
{
	return { offsetX, offsetY };
}

AEVec2 CameraEffects::GetBgOffset() const
{
	return { gravBgOffsetX, gravBgOffsetY };
}