/*!***********************************************************************
\file	PlayerManager.hpp
\author	Sadric Koh
\date	12/01/2026
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief	
holds the class Player which contains player only variables and inherits
from GameObject class from GameObject


\par	Changes: Added file brief and score
**************************************************************************/

#pragma once
#ifndef PLAYER_MANAGER
#define PLAYER_MANAGER

#include "../header/GameObject.hpp"

class Player : public GameObject {
private:
	f32 playerHealth{ 10.f };
	f32 moveSpeed{150.f};
	f32 score{ 0 };
	//set a downforce for psuedo gravity on jump
	f32 downForce;
	bool canJump{ true };

	void UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec);
};

#endif // !PLAYER_MANAGER
