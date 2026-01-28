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

//Player::Player(f32 health ,f32 moveSpeed,f32 jumpForce ,bool isJump ,f32 score , f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos) 
//	: p_playerHealth(health), p_moveSpeed(moveSpeed), p_jumpForce(jumpForce),
//	  p_isJumping(isJump),p_score(score),p_initialPos{xPos,yPos},
//	  initialYPos(zeroVal),initialVelocity(zeroVal),jumpT(zeroVal),
//	  GameObject(mesh,AEVec2{0.f,0.f},0.f,AEVec2{200.f,200.f})
//{}

void Player::InitializePlayer(f32 health, f32 moveSpeed, bool isJump, f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos) {
	p_playerHealth = health;
	p_moveSpeed = moveSpeed;
	p_isJumping = isJump;
	p_score = score;
	p_initialPos.x = xPos;
	p_initialPos.y = yPos;
	UpdatePosition(p_initialPos);
	p_velocityVector.y = zeroVal;
	Player::mesh = mesh;
}

//takes in the current player pos, deltaTime & movement vector to update the player pos
//void Player::UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec)
//{
//	//input handling
//	GameObject::UpdatePosition(pos);
//}

//Takes in the initial value of points for the code to reference during jump state
void Player::StartJump(AEVec2 velocityVec) {
	if (p_isJumping) return;

}
//Math logic behind jump for parabola jump
void Player::PlayerJump(f32 dt) {
	if (!p_isJumping) return;
	p_velocityVector.y += gravity * dt;
	p_initialPos.y += p_velocityVector.y * dt;
	jumpBuffer = 0.f;
	coyoteBuffer = 0.f;
}

void Player::PlayerInputHandle(f32 dt) {
	f32 moveSpeed{ 300.f };
	bool isMoving{ false };
	bool isSprint{ false };

	if (AEInputCheckCurr(AEVK_D)) {
		p_movementVector.x = 1.f * p_moveSpeed * dt;
		p_initialPos.x += p_movementVector.x;
		isMoving = true;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		p_movementVector.x = -1.f * p_moveSpeed * dt;
		p_initialPos.x += p_movementVector.x;
		isMoving = true;
	}
	if (AEInputCheckCurr(AEVK_LSHIFT) && isMoving) {
		isSprint = true;
	}
	if (AEInputCheckReleased(AEVK_LSHIFT)) {
		isSprint = false;
	}

	//Sprint speed logic
	if (isSprint) {
		f32 maxSpeed = moveSpeed * 3.f;
		if (p_moveSpeed <= maxSpeed) {
			p_moveSpeed += moveSpeed * dt;
		}
	}
	else {
		if (p_moveSpeed > moveSpeed) {
			p_moveSpeed -= (moveSpeed)*dt;
		}
		else {
			p_moveSpeed = moveSpeed;
		}
	}

	if (AEInputCheckTriggered(AEVK_SPACE) && 
	   (!GetPlayerJump())) {
		//adjust jump logic here
		p_velocityVector.y = 850.f;
		jumpBuffer = maxJumpBuffer;
		coyoteBuffer = maxCoyoteBuffer;
		p_isJumping = true;
		if (jumpBuffer > 0.f && (p_isGrounded || coyoteBuffer > 0.f)) {
			PlayerJump(dt);
		}
	}
}

//Input handle for player movement logic
void Player::PlayerMovement(f32 dt) {
	PlayerInputHandle(dt);

	jumpBuffer = max(0, jumpBuffer - dt);
	coyoteBuffer = p_isGrounded ? maxCoyoteBuffer : max(0, coyoteBuffer - dt);

	//Collision logic here

	//do some velocityVector Y = 0 check here

	//down force velocity (always going downwards)
	p_velocityVector.y += gravity * dt;
	p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;

	//std::cout << p_velocityVector.y << std::endl;

	if (p_initialPos.y <= -AEGfxGetWindowHeight() + AEGfxGetWinMaxY() + (GetScale().y / 2.f)) {
		std::cout << "grounded" << std::endl;
		p_initialPos.y = GetPosition().y;
		p_velocityVector.y = 0.f;
		p_isJumping = false;
		p_isGrounded = true;
	}

	UpdatePosition(p_initialPos);
}

//sets the player health to smth new depending on parameter
void Player::SetPlayerHealth(f32 healthDamage) {
	p_playerHealth -= healthDamage;
}
//sets the new score on the param
void Player::SetPlayerScore(f32 scoreAdded) {
	p_score += scoreAdded;
}

Collider Player::GetPlayerCollider() const {
	return playerCollider;
}

f32 Player::GetPlayerHealth() const{
	return p_playerHealth;
}
f32 Player::GetPlayerSpeed() const{
	return p_moveSpeed;
}
f32 Player::GetPlayerJumpForce() const{
	return p_downForce;
}
f32 Player::GetPlayerScore() const{
	return p_score;
}
bool Player::GetPlayerJump() const {
	return p_isJumping;
}

