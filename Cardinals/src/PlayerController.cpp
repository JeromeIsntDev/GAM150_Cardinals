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
	p_initialPos.x = xPos;
	p_initialPos.y = yPos;
	UpdatePosition(p_initialPos);
	p_velocityVector.y = zeroVal;
	Player::mesh = mesh;
	p_previousPos = p_initialPos;
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
	if (AEInputCheckCurr(AEVK_D) && canMoveRight) {
		p_velocityVector.x = 1.f * p_moveSpeed * dt;
		p_initialPos.x += p_velocityVector.x;
		isMoving = true;
	}
	if (AEInputCheckCurr(AEVK_A) && canMoveLeft) {
		p_velocityVector.x = -1.f * p_moveSpeed * dt;
		p_initialPos.x += p_velocityVector.x;
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
	std::cout << "jump bool: " << p_isJumping << std::endl;

	if (AEInputCheckCurr(AEVK_SPACE) && !p_isJumping) {
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

//Player Movement Logic
void Player::PlayerMovement(f32 dt) {
	//std::cout << "dt: " << dt << std::endl;
	PlayerInputHandle(dt);

	jumpBuffer = max(0, jumpBuffer - dt);
	coyoteBuffer = p_isGrounded ? maxCoyoteBuffer : max(0, coyoteBuffer - dt);

	//down force velocity (always going downwards)
	if (!p_isGrounded) {
		p_velocityVector.y += gravity * dt;
		p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;
	}
	//if top of player goes past bottom of screen
	if (p_initialPos.y + (GetScale().y * 0.5f) <= -AEGfxGetWindowHeight() + AEGfxGetWinMaxY() + (GetScale().y / 2.f)) {
		p_initialPos = p_previousPos;
		p_velocityVector.y = 0.f;
	}

	UpdatePosition(p_initialPos);
}

void Player::PlayerCollision(std::vector<GameObject*> objs)
{
	for (GameObject* obj : objs) {
		if (this->_tag == obj->_tag) continue;
		if (!CollisionDetected(this->GetCollider(), obj->GetCollider())) continue;

		AEVec2 currentScale = obj->GetScale();
		AEVec2 currentPos = obj->GetPosition();

		this->hasCollided = true;

		f32 overlapX =
			(std::min)(this->GetCollider().p_maxPoint.x, obj->GetCollider().p_maxPoint.x) -
			(std::max)(this->GetCollider().p_minPoint.x, obj->GetCollider().p_minPoint.x);

		f32 overlapY =
			(std::min)(this->GetCollider().p_maxPoint.y, obj->GetCollider().p_maxPoint.y) -
			(std::max)(this->GetCollider().p_minPoint.y, obj->GetCollider().p_minPoint.y);

		if (overlapX <= 0.f || overlapY <= 0.f) continue;

		// Resolve on smallest penetration axis
		if (overlapY <= overlapX + 0.5f)
		{
			AEVec2 tmpPos = this->GetPosition();

			//if player is above collidable
			if (this->GetPosition().y > obj->GetPosition().y)
			{
				tmpPos.y += overlapY;
				this->UpdatePosition(tmpPos);

				this->p_velocityVector.y = 0.f;
				this->p_isGrounded = true;
				this->p_isJumping = false;
			}
			//if player is under collidable
			else
			{
				tmpPos.y -= overlapY;
				this->UpdatePosition(tmpPos);

				if (this->p_velocityVector.y > 0.f)
					this->p_velocityVector.y = 0.f;
			}
		}
		else
		{
			//temp side collision
			AEVec2 tmpPos = this->GetPosition();

			if (this->GetCollider().p_maxPoint.x > obj->GetCollider().p_minPoint.x &&
				this->GetCollider().p_minPoint.x < obj->GetCollider().p_minPoint.x)
			{
				// Player hit object's LEFT side
				tmpPos.x -= overlapX;
				this->canMoveRight = false;
			}
			else
			{
				// Player hit object's RIGHT side
				std::cout << "right hit" << std::endl;
				tmpPos.x += overlapX;
				this->canMoveLeft = false;
			}
			this->UpdatePosition(tmpPos);
			this->p_velocityVector.x = 0.f;
		}
	}
}

//sets the player health to smth new depending on parameter
void Player::SetPlayerHealth(f32 healthDamage) {
	p_playerHealth -= healthDamage;
}

bool Player::GetPlayerCollide() const{
	return hasCollided;
}

void Player::SetPlayerCollide(bool collide) {
	hasCollided = collide;
}

AEVec2 Player::GetPrevPos() const {
	return p_previousPos;
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
bool Player::GetPlayerJump() const {
	return p_isJumping;
}

