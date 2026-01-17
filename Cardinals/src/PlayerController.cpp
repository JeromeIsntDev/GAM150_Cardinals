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
	//initializerList to initialize with values
	: playerHealth(p_health), moveSpeed(p_movespeed),score(0.f), jumpForce(p_jumpForce), canJump(p_canjump)
{
	std::cout << "Player Created" << std::endl;
}
//takes in the current player pos, deltaTime & movement vector to update the player pos
void Player::UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec, AEVec2 velocityVec)
{
	f32 gravity{ 0.f };
	f32 gravityScale{ 1.f };

	//default value to 200.f atm
	f32 maxHeight{ pos.y + 200.f };

	//initialize with 0
	f64* pTime{ 0 };
	//stores the value
	f64 time;
	//input handling
	//Jump logic
	if (AEInputCheckTriggered(AEVK_SPACE) && canJump) {
		//replace this later
		f32 initialHeight{ pos.y };
		time = AEGetTime(pTime);
		velocityVec.y = (2 * (maxHeight * velocityVec.x) / time);
		f32 upper_frac_grav = (-2 * (maxHeight * (velocityVec.x * velocityVec.x)));
		gravity = (upper_frac_grav / (time * time));
		//ax^2 + bx + c formula
		pos.y += ((gravity / 2.f) * time) + (velocityVec.y * time) + (initialHeight);
	}
	if (AEInputCheckCurr(AEVK_D)) {
		movementVec.x = 1.f;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		movementVec.x = -1.f;
	}
	velocityVec.x = movementVec.x * moveSpeed;
	pos.x += velocityVec.x * deltaTime;

	GameObject::UpdatePosition(pos);
}
