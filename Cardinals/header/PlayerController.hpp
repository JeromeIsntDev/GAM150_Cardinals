/**
 * @file PlayerController.hpp
 * @brief The Header holding the class of player and all its function
 * @details -
 * @author Sadric Koh s,koh
 * @date 01.04.2026
 */

#pragma once
#ifndef PLAYER_MANAGER
#define PLAYER_MANAGER

#include "../header/GameObject.hpp"
#include "../header/Physics.hpp"
#include "../header/GridManager.hpp"
#include "../header/AudioManager.hpp"
#include "Animator.hpp"


 /**
  * @class Player
  * @brief Inherited from Gameobject and contains functions
  *	pertaining to player.
  */
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
	/**
	 * @brief Initializes the player with values for the level
	 *
	 * @param health sets the health
	 * @param moveSpeed sets the speed of player
	 * @param isJump to set boolean on startup
	 * @param score set the score
	 * @param zeroVal parse in a 0
	 * @param mesh sets the mesh of the player
	 * @param xPos sets the starting position X
	 * @param yPos sets the starting position Y
	 * @param gOver sets gameover boolean
	 */
	void InitializePlayer(int health, f32 moveSpeed, bool isJump ,f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos, bool gOver);
	/**
	 * @brief Getter for health
	 *
	 * @return returns player health
	 */
	int GetPlayerHealth() const;
	/**
	 * @brief Getter for Speed
	 *
	 * @return returns player speed
	 */
	f32 GetPlayerSpeed() const;
	/**
	 * @brief Getter for player Jumpforce
	 *
	 * @return returns player Jumpforce
	 */
	f32 GetPlayerJumpForce() const;
	/**
	 * @brief Getter for boolean of playerJump
	 *
	 * @return returns boolean
	 */
	bool GetPlayerJump() const;
	/**
	 * @brief Getter for boolean of player collision
	 *
	 * @return return boolean
	 */
	bool GetPlayerCollide() const;
	/**
	 * @brief Getter for previous position of player
	 *
	 * @return return Vector2 of player prev position
	 */
	AEVec2 GetPrevPos() const;
	/**
	 * @brief Getter for player velocity
	 *
	 * @return return Vector2 of player velocity
	 */
	AEVec2 GetVelocity() const;
	/**
	 * @brief Getter for Gameover boolean
	 *
	 * @return return boolean
	 */
	bool getGameOver() const;
	/**
	 * @brief Setter for player collision
	 * 
	 * @param bool to be parsed in to set playerCollide
	 */
	void SetPlayerCollide(bool);
	/**
	 * @brief Setter for gameOver
	 *
	 * @param bool to be parsed in to set gameOver
	 */
	void SetGameOver(bool);
	/**
	 * @brief Setter for player health
	 *
	 * @param int to be parsed in to set playerHealth
	 */
	void SetHealth(int);
	/**
	 * @brief Handles the input given by the player
	 *
	 * @param const f32& to parse in the deltaTime
	 * @param GravityDirection& to parse in the direction of player
	 * @param Grid& to parse in the grid of the map
	 */
	void PlayerInputHandle(f32 const&, GravityDirection&, Grid&);
	/**
	 * @brief Subset of InputHandle to handle input when grav is up or down
	 *
	 * @param dt to take the dt to calculate movement
	 */
	void PlayerInputGravUpDown(const f32& dt);
	/**
	 * @brief Subset of InputHandle to handle input when grav is left or right
	 *
	 * @param dt to take the dt to calculate movement
	 */
	void PlayerInputGravLeftRight(const f32& dt);
	/**
	 * @brief Subset of InputHandle to handle player sprint
	 */
	void PlayerInputSprint();
	/**
	 * @brief Rotation anim of player when player dies
	 *
	 * @param dt to take the dt to calculate timing
	 */
	void PlayerRotAnim(const f32& dt);
	/**
	 * @brief Subset of InputHandle to handle jump inputs
	 *
	 * @param dt to take the dt to calculate movement
	 * @param GravityDirection& to check the direction of player
	 */
	void PlayerInputJump(const f32& dt, GravityDirection&);
	/**
	 * @brief Subset of InputHandle to handle input of collecting item
	 *
	 * @param Grid& to take in the grid to check with the map
	 */
	void PlayerInputCollectible(Grid&);
	/**
	 * @brief The Math function to provide movement to player
	 *
	 * @param dt to take the dt to calculate movement
	 * @param GravityDirection& to check the player direction
	 * @param grid to take in the map and check with the map
	 */
	void PlayerMovement(f32 const&, GravityDirection& dir, Grid& grid);
	/**
	 * @brief Checks with all collidable object
	 *
	 * @param objs object vector to be checked with
	 */
	void PlayerCollision(std::vector<GameObject*> objs);
	/**
	 * @brief Gravity application on the player
	 *
	 * @param dt to take the dt to calculate movement
	 * @param GravityDirection& to check the player direction
	 */
	void GravityApplication(const f32&, GravityDirection&);
	/**
	 * @brief Checks if player goes out of bounds of the window
	 */
	void WindowBoundsCheck();
	/**
	 * @brief Binary Collision with the map data
	 *
	 * @param grid to check collision with the map
	 * @param GravityDirection& to check with the player direction
	 */
	void CheckBinaryCollision(Grid* grid, GravityDirection&);
	/**
	 * @brief Checks the player position and whether its colliding
	 *
	 * @param PosX the xPos of the player
	 * @param PosY the yPos of the player
	 * @param scaleX the xScale of the player
	 * @param scaleY the yScale of the player
	 * @param grid the map data
	 * 
	 * @return the Collision value if colliding
	 */
	int CheckInstanceBinaryMapCollision(f32 PosX, f32 PosY, f32 scaleX, f32 scaleY, Grid* grid);
	/**
	 * @brief Additional Jump Logic
	 *
	 * @param dt to take the dt to calculate movement
	 * @param GravityDirection& the direction of the player
	 */
	void PlayerJump(f32 const&, GravityDirection&);
	/**
	 * @brief to fix certain issues
	 */
	Player();
};

const unsigned int	COLLISION_LEFT = 0x00000001;		//0001
const unsigned int	COLLISION_RIGHT = 0x00000002;		//0010
const unsigned int	COLLISION_TOP = 0x00000004;			//0100
const unsigned int	COLLISION_BOTTOM = 0x00000008;		//1000


#endif // !PLAYER_MANAGER
