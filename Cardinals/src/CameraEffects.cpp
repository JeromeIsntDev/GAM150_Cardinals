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

void CameraEffects::Init()
{
	offsetX = 0.f;
	offsetY = 0.f;
	shakeTimeLeft = 0.f;
	shakeMagnitude = 0.f;
	AEGfxSetCamPosition(0.f, 0.f);
}

void CameraEffects::Update(f32 dt)
{
	offsetX = 0.f;
	offsetY = 0.f;

	if (shakeTimeLeft > 0.f)
	{
		shakeTimeLeft -= dt;

		offsetX = (((rand() % 100) / 50.f) - 1.f) * shakeMagnitude;
		offsetY = (((rand() % 100) / 50.f) - 1.f) * shakeMagnitude;

		if (shakeTimeLeft < 0.f)
		{
			shakeTimeLeft = 0.f;
		}
	}
}

void CameraEffects::Apply() const
{
	AEGfxSetCamPosition(offsetX, offsetY);
}

void CameraEffects::TriggerShake(f32 magnitude, f32 duration)
{
	shakeMagnitude = magnitude;
	shakeTimeLeft = duration;
}

void CameraEffects::TriggerShake()
{
	TriggerShake(defaultShakeMagnitude, defaultShakeDuration);
}

void CameraEffects::Clear()
{
	offsetX = 0.f;
	offsetY = 0.f;
	shakeTimeLeft = 0.f;
	shakeMagnitude = 0.f;
	AEGfxSetCamPosition(0.f, 0.f);
}

AEVec2 CameraEffects::GetOffset() const
{
	return { offsetX, offsetY };
}