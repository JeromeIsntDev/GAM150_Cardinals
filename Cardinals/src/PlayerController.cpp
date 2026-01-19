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

Player::Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_canjump, AEGfxVertexList* mesh)
	: p_playerHealth(p_health), p_moveSpeed(p_movespeed), p_score(0.f), p_jumpForce(p_jumpForce), p_canJump(p_canjump),
	GameObject(mesh, AEVec2{ 0.f,0.f }, 0.f, AEVec2{ 200.f,200.f })
{
	std::cout << "Player Created with PosX: " << globalPosition.x << " Pos Y: " << globalPosition.y
	<< " Health: " << p_playerHealth << " Speed: " << p_moveSpeed
	<< std::endl;
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
	if (AEInputCheckTriggered(AEVK_SPACE) && p_canJump) {
		//replace this later
		f32 initialHeight{ pos.y };
		time = AEGetTime(pTime);
		velocityVec.y = (2 * (maxHeight * velocityVec.x) / static_cast<f32>(time));
		f32 upper_frac_grav = (-2 * (maxHeight * (velocityVec.x * velocityVec.x)));
		gravity = (upper_frac_grav / static_cast<f32>((time * time)));
		//ax^2 + bx + c formula
		pos.y += (((gravity * gravityScale) / 2.f) * static_cast<f32>(time)) + (velocityVec.y * static_cast<f32>(time)) + (initialHeight);
	}
	if (AEInputCheckCurr(AEVK_D)) {
		movementVec.x = 1.f;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		movementVec.x = -1.f;
	}
	velocityVec.x = movementVec.x * p_moveSpeed;
	pos.x += velocityVec.x * deltaTime;

	GameObject::UpdatePosition(pos);
}

void Player::PlayerJump() {

}

//sets the player health to smth new depending on parameter
void Player::SetPlayerHealth(f32 healthDamage) {
	p_playerHealth -= healthDamage;
}
//sets the new score on the param
void Player::SetPlayerScore(f32 scoreAdded) {
	p_score += scoreAdded;
}

f32 Player::GetPlayerHealth() const{
	return p_playerHealth;
}
f32 Player::GetPlayerSpeed() const{
	return p_moveSpeed;
}
f32 Player::GetPlayerJumpForce() const{
	return p_jumpForce;
}
f32 Player::GetPlayerScore() const{
	return p_score;
}

