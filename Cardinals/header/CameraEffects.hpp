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

#include <AEEngine.h>

class CameraEffects {
private:
	f32 offsetX{ 0.f };
	f32 offsetY{ 0.f };

	f32 shakeTimeLeft{ 0.f };
	f32 shakeMagnitude{ 0.f };

	f32 defaultShakeMagnitude{ 1.3f };
	f32 defaultShakeDuration{ 0.2f };

	//f32 springX{ 0.f };
	//f32 springY{ 0.f };
	//f32 springStrength{ 0.f };
	//f32 springDamping{ 0.f };

public:
	void Init();
	void Update(f32 dt);
	void Apply() const;
	void Clear();

	void TriggerShake();
	void TriggerShake(f32 magnitude, f32 duration);

	AEVec2 GetOffset() const;
};

#endif
