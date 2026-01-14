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
	
	Player() = delete;

	f32 playerHealth;
	f32 moveSpeed;
	f32 score;
	//set a downforce for psuedo gravity on jump
	f32 jumpForce;
	bool canJump;

	Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_canjump);

	void UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec);
};

#endif // !PLAYER_MANAGER
