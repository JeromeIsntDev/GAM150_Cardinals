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
private:
	f32 p_moveSpeed;
	f32 p_score;
	//set a jumpforce for psuedo gravity on jump
	f32 p_jumpForce;
	bool p_canJump;
	f32 p_playerHealth;
public:
	//initializes f32 Health,movespeed,jumpforce,boolean canJump
	Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_canjump);
	//Update state loop
	void UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec, AEVec2 velocityVec);
	//Gets the player health
	f32 GetPlayerHealth();
	f32 GetPlayerSpeed();
	f32 GetPlayerJumpForce();
	f32 GetPlayerScore();
	//set the player health
	void SetPlayerHealth(f32 healthDamage);
	void SetPlayerScore(f32 scoreAdded);
};

#endif // !PLAYER_MANAGER
