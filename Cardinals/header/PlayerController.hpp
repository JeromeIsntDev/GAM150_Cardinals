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
	//Player() = delete;
private:
	f32 p_playerHealth;
	f32 p_moveSpeed;
	f32 p_jumpForce;
	bool p_isJumping;
	f32 p_score;
	//set a jumpforce for psuedo gravity on jump
	const f32 gravity{ -986.f };
	const f32 maxHeight{ 200.f };
	AEGfxVertexList* p_mesh;
public:
	//Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_isJump, AEGfxVertexList* mesh);

	//public Variable for Player (Check if error)
	f32 initialYPos;
	f32 initialVelocity;
	f32 jumpT;

	//void UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec);
	//Gets the player data

	void InitializePlayer(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh);

	f32 GetPlayerHealth() const;
	f32 GetPlayerSpeed() const;
	f32 GetPlayerJumpForce() const;
	f32 GetPlayerScore() const;
	bool GetPlayerJump() const;
	//set the player health
	void SetPlayerHealth(f32 healthDamage);
	void SetPlayerScore(f32 scoreAdded);

	void PlayerMovement(f32 dt, f32 jumpForce);
	//Function for Player Jump Logic
	//Basically the initialize values for PreJumps
	void StartJump(AEVec2 velocityVec, f32 jumpForce);
	//The math and update for during the jump
	void PlayerJump(f32 dt);
};

#endif // !PLAYER_MANAGER
