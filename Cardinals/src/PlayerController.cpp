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

Player::Player(f32 p_health, f32 p_movespeed, f32 p_jumpForce, bool p_isJump, AEGfxVertexList* mesh)
	: p_playerHealth(p_health), p_moveSpeed(p_movespeed), p_score(0.f), p_jumpForce(p_jumpForce), p_isJumping(p_isJump),
	GameObject(mesh, AEVec2{ 0.f,0.f }, 0.f, AEVec2{ 200.f,200.f })
{
	std::cout << "Player Created with PosX: " << globalPosition.x << " Pos Y: " << globalPosition.y
	<< " Health: " << p_playerHealth << " Speed: " << p_moveSpeed
	<< std::endl;	
}


//takes in the current player pos, deltaTime & movement vector to update the player pos
//void Player::UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec)
//{
//	//input handling
//	GameObject::UpdatePosition(pos);
//}

void Player::StartJump() {
	if (p_isJumping) return;
	initialYPos = GetPosition().y;
	initialVelocity = std::sqrt(2.f * (-gravity) * maxHeight);
	jumpT = 0.f;
	p_isJumping = true;
}

void Player::PlayerJump(f32 dt) {
	if (!p_isJumping) return;
	jumpT += dt;
	GetPosition().y = initialYPos + initialVelocity * jumpT + 0.5f * gravity * (jumpT * jumpT);

	if (GetPosition().y <= initialYPos) {
		GetPosition().y = initialYPos;
		p_isJumping = false;
	}

}


void Player::PlayerMovement(f32 dt) {
	AEVec2 movementVector{ 0.f,0.f };
	AEVec2 velocityVector{ 0.f,0.f };
	f32 moveSpeed{ 300.f };

	if (AEInputCheckCurr(AEVK_D)) {
		movementVector.x = 1.f * moveSpeed * dt;
		GetPosition().x += movementVector.x;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		movementVector.x = -1.f * moveSpeed * dt;
		GetPosition().x += movementVector.x;
	}
	if (AEInputCheckTriggered(AEVK_SPACE) && !Player::GetPlayerJump()) {
		std::cout << "Jumped" << std::endl;
		velocityVector.y = 1.f;
		StartJump();
	}
	PlayerJump(dt);
	Player::UpdatePosition(GetPosition());
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
bool Player::GetPlayerJump() const {
	return p_isJumping;
}

