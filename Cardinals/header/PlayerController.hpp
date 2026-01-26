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
	f32 p_playerHealth{ 100.f };
	f32 p_moveSpeed{ 300.f };
	f32 p_downForce{ 0.5f };
	bool p_isJumping{ false };
	bool p_isInverted{ false };
	bool p_isFalling{ true };
	f32 p_score{ 0.f };
	//set a jumpforce for psuedo gravity on jump
	f32 gravity{ -986.f };
	const f32 maxHeight{ 200.f };
	AEVec2 p_initialPos{ 0.f,0.f };
	f32 p_leftMin{ 0.f };
	f32 p_rightMax{ 0.f };
	f32 p_displacement{ 0.f };
	AEVec2 p_movementVector{ 0.f,0.f };
	AEVec2 p_velocityVector{ 0.f,0.f };
public:
	//Player(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);

	//public Variable for Player (Check if error)
	f32 initialYPos{ 0.f };
	f32 initialVelocity{ 0.f };
	f32 jumpT{ 0.f };

	//void UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec);
	//Gets the player data

	void InitializePlayer(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);

	f32 GetPlayerHealth() const;
	f32 GetPlayerSpeed() const;
	f32 GetPlayerJumpForce() const;
	f32 GetPlayerScore() const;
	bool GetPlayerJump() const;
	//set the player health
	void SetPlayerHealth(f32);
	void SetPlayerScore(f32);
	void PlayerInputHandle(f32);
	void PlayerMovement(f32, f32);
	//Function for Player Jump Logic
	//Basically the initialize values for PreJumps
	void StartJump(AEVec2);
	//The math and update for during the jump
	void PlayerJump(f32);
};


#endif // !PLAYER_MANAGER
