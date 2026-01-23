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

void Player::InitializePlayer(f32 health, f32 moveSpeed, f32 jumpForce, bool isJump, f32 score, f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos) {
	p_playerHealth = health;
	p_moveSpeed = moveSpeed;
	p_jumpForce = jumpForce;
	p_isJumping = isJump;
	p_score = score;
	initialYPos = initialVelocity = jumpT = zeroVal;
	p_initialPos.x = xPos;
	p_initialPos.y = yPos;
	UpdatePosition(p_initialPos);
	Player::mesh = mesh;
}

//takes in the current player pos, deltaTime & movement vector to update the player pos
//void Player::UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec)
//{
//	//input handling
//	GameObject::UpdatePosition(pos);
//}

//Takes in the initial value of points for the code to reference during jump state
void Player::StartJump(AEVec2 velocityVec, f32 jumpForce) {
	if (p_isJumping) return;

	initialYPos = GetPosition().y;
	f32 jumpSpeed = velocityVec.y * (std::sqrt(2.f * (-gravity) * maxHeight));
	f32 direction = p_isInverted ? -1.f : 1.f;
	initialVelocity = direction * jumpSpeed;
	jumpT = 0.f;
	p_jumpForce = jumpForce;
	p_isJumping = true;
}
//Math logic behind jump for parabola jump
void Player::PlayerJump(f32 dt) {
	if (!p_isJumping) return;
	jumpT += dt;
	p_jumpForce += dt * 0.2f;
	std::cout << "Jump Force: " << p_jumpForce << std::endl;
	//The Update for the Y position for Parabola
	GetPosition().y = (initialYPos + initialVelocity * jumpT + p_jumpForce * gravity * (jumpT * jumpT));
	//If displacement is lower than initial, stop jumping (Change this logic in the future)
	f32 displacement = GetPosition().y - initialYPos;
	if ((!p_isInverted && displacement <= 0.f) || (p_isInverted && displacement >= 0.f)){
		//fix this logic
		GetPosition().y = initialYPos;
		p_isJumping = false;
	}
}
//Input handle for player movement logic
void Player::PlayerMovement(f32 dt, f32 jumpForce) {
	//always reset to 0 value
	AEVec2 movementVector{ 0.f,0.f };
	AEVec2 velocityVector{ 0.f,0.f };
	f32 moveSpeed{ 300.f };
	bool isMoving{ false };
	bool isSprint{ false };

	if (AEInputCheckCurr(AEVK_D)) {
		movementVector.x = 1.f * p_moveSpeed * dt;
		GetPosition().x += movementVector.x;
		isMoving = true;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		movementVector.x = -1.f * p_moveSpeed * dt;
		GetPosition().x += movementVector.x;
		isMoving = true;
	}
	if (AEInputCheckCurr(AEVK_LSHIFT) && isMoving) {	
		isSprint = true;
	}
	if (AEInputCheckReleased(AEVK_LSHIFT)) {
		isSprint = false;
	}
	//if (AEInputCheckTriggered(AEVK_Q)) {
	//	//toggle
	//	p_isInverted = !p_isInverted;
	//}
	//Temp sprint up and down logic
	if (isSprint) {
		f32 maxSpeed = moveSpeed * 3.f;
		if (p_moveSpeed <= maxSpeed) {
			p_moveSpeed += moveSpeed * dt;
		}
	}else {
		if (p_moveSpeed > moveSpeed) {
			p_moveSpeed -= (moveSpeed) * dt;
		}
		else {
			p_moveSpeed = moveSpeed;
		}
	}

	std::cout << "speed: " << p_moveSpeed << std::endl;
	if (AEInputCheckTriggered(AEVK_SPACE) && !Player::GetPlayerJump()) {
		std::cout << "Jumped" << std::endl;
		//adjust for jump height
		velocityVector.y = 1.f;
		StartJump(velocityVector, jumpForce);
	}
	PlayerJump(dt);
	UpdatePosition(GetPosition());
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

