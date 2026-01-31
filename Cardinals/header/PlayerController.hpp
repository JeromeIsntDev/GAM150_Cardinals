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
#include "../header/Physics.hpp"

class Player : public GameObject {
	//Player() = delete;
private:
	f32 p_playerHealth{ 100.f };
	f32 p_moveSpeed{ 300.f };
	f32 p_downForce{ 1.5f };
	bool p_isInverted{ false };
	f32 p_score{ 0.f };
	//set a jumpforce for psuedo gravity on jump
	f32 gravity{ -2000.f };
	AEVec2 p_initialPos{ 0.f,0.f };
	f32 p_leftMin{ 0.f };
	f32 p_rightMax{ 0.f };
	AEVec2 p_movementVector{ 0.f,0.f };
	AEVec2 p_previousPos{ 0.f,0.f };
public:
	//Player(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);

	AEVec2 p_velocityVector{ 0.f,0.f };
	bool p_isJumping{ true };
	bool left{ true };
	bool right{ true };
	//public Variable for Player (Check if error)
	bool p_isGrounded{ false };
	f32 maxJumpBuffer{ 0.12f };
	f32 jumpBuffer{ 0.f };
	f32 maxCoyoteBuffer{ 0.8f };
	f32 coyoteBuffer{ 0.f };

	bool hasCollided{ false };
	//void UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec);
	//Gets the player data

	void InitializePlayer(f32 health, f32 moveSpeed, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);

	f32 GetPlayerHealth() const;
	f32 GetPlayerSpeed() const;
	f32 GetPlayerJumpForce() const;
	f32 GetPlayerScore() const;
	bool GetPlayerJump() const;
	bool GetPlayerCollide() const;
	AEVec2 GetPrevPos() const;
	//set the player health
	void SetPlayerCollide(bool);
	void SetPlayerHealth(f32);
	void SetPlayerScore(f32);
	void PlayerInputHandle(f32);
	void PlayerMovement(f32);
	//Function for Player Jump Logic
	//Basically the initialize values for PreJumps
	void StartJump(AEVec2);
	//The math and update for during the jump
	void PlayerJump(f32);
};


#endif // !PLAYER_MANAGER
