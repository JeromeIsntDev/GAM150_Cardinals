/*!***********************************************************************
\file	PlayerManager.cpp
\author	Sadric Koh
\date	12/01/2026
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief	The Manager that holds everything related to player and will control
the player movement etc

\par	Changes: Added Player Constructor
**************************************************************************/

#include "../header/PlayerController.hpp"
#include <iostream>

//initialize player with values
Player::Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_canjump)
	: playerHealth(p_health), moveSpeed(p_movespeed),score(0.f), jumpForce(p_jumpForce), canJump(p_canjump)
{
	std::cout << "Player Created" << std::endl;
}
//takes in the current player pos, deltaTime & movement vector to update the player pos
void Player::UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec)
{
	pos.x += movementVec.x * moveSpeed * deltaTime;
	//possible change to this Y movement
	if (AEInputCheckTriggered(AEVK_SPACE) && canJump) {
		//replace this later
		pos.y += movementVec.y * moveSpeed * deltaTime;
	}

	GameObject::UpdatePosition(pos);
}
