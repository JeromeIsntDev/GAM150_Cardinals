/*!***********************************************************************
\file	PlayerManager.hpp
\author	Sadric Koh
\date	12/01/2026
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief	
holds the class Player which contains player only variables and inherits
from GameObject class from GameObject


\par	Changes: Added Player initializer
**************************************************************************/

#pragma once
#ifndef PLAYER_MANAGER
#define PLAYER_MANAGER

#include "../header/GameObject.hpp"

class Player : public GameObject {
	Player() = delete;

	
	f32 moveSpeed;
	f32 score;
	//set a jumpforce for psuedo gravity on jump
	f32 jumpForce;
	bool canJump;
	//Constructor
public:
	f32 playerHealth;
	Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_canjump);

	void UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec, AEVec2 velocityVec);
};

#endif // !PLAYER_MANAGER
