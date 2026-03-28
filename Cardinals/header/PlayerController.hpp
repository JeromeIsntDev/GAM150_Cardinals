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
#include "../header/GridManager.hpp"
#include "../header/AudioManager.hpp"
#include "Animator.hpp"

class Player : public GameObject {

	//Player() = delete;
private:
	int p_playerHealth{ 3 };
	f32 p_moveSpeed{ 300.f };
	f32 p_downForce{ 1.5f };
	//set a jumpforce for psuedo gravity on jump
	f32 gravity{ 1500.f };
	AEVec2 p_currPos{ 0.f,0.f };
	AEVec2 p_previousPos{ 0.f,0.f };
	f32 maxCD{ 0.4f };
	f32 currCD{ 0.f };
	f32 changeCD{ 0.f };
	f32 jumpForce = 1250.f;
	bool gameOver = false;

	AudioManager& bgm = AudioManager::Instance();
public:
	//Player(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos);
	AEVec2 p_Scale{ 50.f,50.f };

	enum class GravityDirection {
		Up, Down, Left, Right,
		GravNum
	};
	GravityDirection prevDir = GravityDirection::Down;
	AEVec2 p_velocityVector{ 0.f,0.f };
	f32 playerScore{ 0 };
	Animator newAnim;
	bool p_isJumping{ true };
	bool canMoveLeft{ true };
	bool canMoveRight{ true };
	bool canMoveUp{ false };
	bool canMoveDown{ false };
	bool canMove{ true };
	bool canSwapGrav = true;
	bool isHit{ false };
	bool gravTouched{ false };
	bool canTouch{ false };
	bool itemTouched{ false };
	//public Variable for Player (Check if error)
	bool p_isGrounded{ false };
	f32 maxJumpBuffer{ 0.12f };
	f32 jumpBuffer{ 0.f };
	f32 maxCoyoteBuffer{ 0.1f };
	f32 coyoteBuffer{ 0.f };
	GravityDirection grav = Player::GravityDirection::Down;
	bool hasCollided{ false };
	bool audioPlayed{ false };
	bool isMoving{ false };
	bool isSprint{ false };

	//Death psuedo animation
	f32 rot_Timer{ 0.f };
	f32 hitCD{ 0.2f };
	int rot_count{ 0 };
	bool hitAnim{ false };
	f32 gravCD{ 0.3f };

	//Gets the player data

	int	gridCollisionFlag{};

	void InitializePlayer(int health, f32 moveSpeed, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos, bool gOver);

	int GetPlayerHealth() const;
	f32 GetPlayerSpeed() const;
	f32 GetPlayerJumpForce() const;
	bool GetPlayerJump() const;
	bool GetPlayerCollide() const;
	AEVec2 GetPrevPos() const;
	AEVec2 GetVelocity() const;
	bool getGameOver() const;
	//set the player health
	void SetPlayerCollide(bool);
	void SetGameOver(bool);
	void SetHealth(int);
	//void SetPlayerHealth(f32);
	void PlayerInputHandle(f32 const&, GravityDirection&);
	void PlayerMovement(f32 const&, GravityDirection& dir);
	void PlayerCollision(std::vector<GameObject*> objs);
	void CheckBinaryCollision(Grid* grid, GravityDirection&);
	int CheckInstanceBinaryMapCollision(f32 PosX, f32 PosY, f32 scaleX, f32 scaleY, Grid* grid);
	//Function for Player Jump Logic
	//Basically the initialize values for PreJumps
	//void StartJump(AEVec2);
	//The math and update for during the jump
	void PlayerJump(f32 const&, GravityDirection&);

	Player();
};

const unsigned int	COLLISION_LEFT = 0x00000001;		//0001
const unsigned int	COLLISION_RIGHT = 0x00000002;		//0010
const unsigned int	COLLISION_TOP = 0x00000004;			//0100
const unsigned int	COLLISION_BOTTOM = 0x00000008;		//1000


#endif // !PLAYER_MANAGER
