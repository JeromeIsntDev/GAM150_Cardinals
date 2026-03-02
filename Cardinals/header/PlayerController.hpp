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
	int p_playerHealth{ 3 };
	f32 p_moveSpeed{ 300.f };
	f32 p_downForce{ 1.5f };
	//set a jumpforce for psuedo gravity on jump
	f32 gravity{ 1500.f };
	AEVec2 p_initialPos{ 0.f,0.f };
	AEVec2 p_previousPos{ 0.f,0.f };
	f32 maxCD{ 0.4f };
	f32 currCD{ 0.f };
	f32 changeCD{ 0.f };
	f32 jumpForce = 500.f;
	bool gameOver = false;
public:
	//Player(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);
	AEVec2 p_Scale{ 50.f,50.f };

	enum class GravityDirection {
		Up, Down, Left, Right,
		GravNum
	};
	GravityDirection prevDir = GravityDirection::Down;
	AEVec2 p_velocityVector{ 0.f,0.f };
	AEVec2 init_Pos{ 0.f,0.f };
	f32 playerScore{ 0 };
	bool p_isJumping{ true };
	bool canMoveLeft{ true };
	bool canMoveRight{ true };
	bool canMoveUp{ false };
	bool canMoveDown{ false };
	bool canSwapGrav = true;
	//public Variable for Player (Check if error)
	bool p_isGrounded{ false };
	f32 maxJumpBuffer{ 0.12f };
	f32 jumpBuffer{ 0.f };
	f32 maxCoyoteBuffer{ 0.1f };
	f32 coyoteBuffer{ 0.f };
	GravityDirection grav = Player::GravityDirection::Down;
	bool hasCollided{ false };
	//void UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec);
	//Gets the player data

	void InitializePlayer(f32 health, f32 moveSpeed, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);

	f32 GetPlayerHealth() const;
	f32 GetPlayerSpeed() const;
	f32 GetPlayerJumpForce() const;
	bool GetPlayerJump() const;
	bool GetPlayerCollide() const;
	bool IsGameOver() const;
	AEVec2 GetPrevPos() const;
	bool getGameOver() const;
	//set the player health
	void SetPlayerCollide(bool);
	void SetGameOver(bool);
	void SetHealth(int);
	void SetPlayerHealth(f32);
	void PlayerInputHandle(f32&, GravityDirection&);
	void PlayerMovement(f32&, GravityDirection& dir);
	void PlayerCollision(std::vector<GameObject*> objs,GravityDirection&);
	//Function for Player Jump Logic
	//Basically the initialize values for PreJumps
	//void StartJump(AEVec2);
	//The math and update for during the jump
	void PlayerJump(f32&, GravityDirection&);
};


#endif // !PLAYER_MANAGER
