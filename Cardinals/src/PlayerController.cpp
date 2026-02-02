/*!***********************************************************************
\file	PlayerManager.cpp
\author	Sadric Koh
\date	12/01/2026
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief	The Manager that holds everything related to player and will control
the player movement etc

\par	Changes: Added Physics stuff
**************************************************************************/

#include "../header/PlayerController.hpp"
#include "../header/InputHandler.hpp"
#include <iostream>

//Player::Player(f32 health ,f32 moveSpeed,f32 jumpForce ,bool isJump ,f32 score , f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos) 
//	: p_playerHealth(health), p_moveSpeed(moveSpeed), p_jumpForce(jumpForce),
//	  p_isJumping(isJump),p_score(score),p_initialPos{xPos,yPos},
//	  initialYPos(zeroVal),initialVelocity(zeroVal),jumpT(zeroVal),
//	  GameObject(mesh,AEVec2{0.f,0.f},0.f,AEVec2{200.f,200.f})
//{}

auto& input = InputHandler::GetInstance();

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
void Player::PlayerJump(f32& dt, GravityDirection& dir) {
	if (!p_isJumping) return;
	switch (dir) {
	case GravityDirection::Up:
	case GravityDirection::Down:
		p_velocityVector.y += gravity * dt;
		p_initialPos.y += p_velocityVector.y * dt;
		break;
	case GravityDirection::Left:
	case GravityDirection::Right:
		p_velocityVector.x += gravity * dt;
		p_initialPos.x += p_velocityVector.x * dt;
		break;
	}
	
	jumpBuffer = 0.f;
	coyoteBuffer = 0.f;
}

void Player::PlayerInputHandle(f32& dt, GravityDirection& dir) {
	f32 moveSpeed{ 300.f };
	bool isMoving{ false };
	bool isSprint{ false };
	if (input.isKeyPressed(AEVK_D)) {
		if (canMoveRight) {
			switch (dir) {
			case GravityDirection::Up:
			case GravityDirection::Down:
				p_velocityVector.x = 1.f * p_moveSpeed * dt;
				p_initialPos.x += p_velocityVector.x;
				break;
			case GravityDirection::Left:
			case GravityDirection::Right:
				p_velocityVector.y = -1.f * p_moveSpeed * dt;
				p_initialPos.y += p_velocityVector.y;
				break;
			default:
				break;
			}
		}
		
		isMoving = true;
	}
	if (input.isKeyPressed(AEVK_A)) {
		if (canMoveLeft) {
			switch (dir) {
			case GravityDirection::Up:
			case GravityDirection::Down:
				p_velocityVector.x = -1.f * p_moveSpeed * dt;
				p_initialPos.x += p_velocityVector.x;
				break;
			case GravityDirection::Left:
			case GravityDirection::Right:
				p_velocityVector.y = 1.f * p_moveSpeed * dt;
				p_initialPos.y += p_velocityVector.y;
				break;
			default:
				break;
			}
		}
		
		isMoving = true;
	}
	if (input.isKeyPressed(AEVK_LSHIFT) && isMoving) {
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
	std::cout << "left bool: " << canMoveLeft << std::endl;
	std::cout << "right bool: " << canMoveRight << std::endl;
	if (input.isKeyPressed(AEVK_SPACE) && !p_isJumping) {
		//adjust jump logic here
		switch(dir){
		case Player::GravityDirection::Up:
			p_velocityVector.y = -850.f;
			break;
		case Player::GravityDirection::Down:
			p_velocityVector.y = 850.f;
			break;
		case Player::GravityDirection::Left:
			p_velocityVector.x = 850.f;
			break;
		case Player::GravityDirection::Right:
			p_velocityVector.x = -850.f;
			break;
		default:
			break;
		}
		jumpBuffer = maxJumpBuffer;
		coyoteBuffer = maxCoyoteBuffer;
		p_isJumping = true;
		if (jumpBuffer > 0.f && (p_isGrounded || coyoteBuffer > 0.f)) {
			PlayerJump(dt,dir);
		}
	}
}

//Player Movement Logic
void Player::PlayerMovement(f32& dt, GravityDirection& dir) {
	//std::cout << "dt: " << dt << std::endl;

	jumpBuffer = max(0, jumpBuffer - dt);
	coyoteBuffer = p_isGrounded ? maxCoyoteBuffer : max(0, coyoteBuffer - dt);
	GravityDirection currDir = dir;

	//CLAMP HERE

	//down force velocity (always going downwards) depend on location
	if (!p_isGrounded) {
		switch (dir)
		{
		case Player::GravityDirection::Up:
			p_velocityVector.y += gravity * dt;
			p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;
			p_initialPos.x += (p_velocityVector.x * p_downForce) * dt;
			break;
		case Player::GravityDirection::Down:
			p_velocityVector.y += -gravity * dt;
			p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;
			p_initialPos.x += (p_velocityVector.x * p_downForce) * dt;

			break;
		case Player::GravityDirection::Left:
			p_velocityVector.x += -gravity * dt;
			p_initialPos.x += (p_velocityVector.x * p_downForce) * dt;
			p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;
			break;
		case Player::GravityDirection::Right:
			p_velocityVector.x += gravity * dt;
			p_initialPos.x += (p_velocityVector.x * p_downForce) * dt;
			p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;
			break;
		default:
			break;
		}
		/*p_velocityVector.y += gravity * dt;
		p_initialPos.y += (p_velocityVector.y * p_downForce) * dt;*/
	}
	std::cout << "X: " << p_velocityVector.x << "Y: " << p_velocityVector.y << std::endl;
	//resets velocity to avoid random snapping or teleporting
	if (currDir != prevDir) {
		currCD += dt;
		p_isJumping = true;
		if (currCD >= maxCD) {
			p_isJumping = false;
			currCD = 0.f;
		}
		//p_velocityVector = { 0.f,0.f };
		prevDir = currDir;
	}
	PlayerInputHandle(dt, dir);
	//if top of player goes past bottom of screen (temporary stuff, probably wont need)
	if (p_initialPos.y + (GetScale().y * 0.5f) <= -AEGfxGetWindowHeight() + AEGfxGetWinMaxY() + (GetScale().y / 2.f)) {
		p_initialPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		grav = Player::GravityDirection::Down;
	}
	if (p_initialPos.y >= 400.f) {
		p_initialPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		grav = Player::GravityDirection::Down;
	}
	if (p_initialPos.x >= 750.f) {
		p_initialPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		grav = Player::GravityDirection::Down;
	}
	if (p_initialPos.x <= -750.f) {
		p_initialPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		grav = Player::GravityDirection::Down;
	}

	PRINT("Momentum: %f,%f", p_velocityVector.x, p_velocityVector.y);

	UpdatePosition(p_initialPos);
}

void Player::PlayerCollision(std::vector<GameObject*> objs, GravityDirection& dir)
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

		// Resolve on smallest penetration axis (Y axis)
		if (overlapY <= overlapX + 0.5f)
		{
			AEVec2 tmpPos = this->GetPosition();
			switch (dir) {
				case Player::GravityDirection::Up:
					if (this->GetPosition().y < obj->GetPosition().y)
					{
						tmpPos.y -= overlapY;
						this->UpdatePosition(tmpPos);

						this->p_velocityVector.y = 0.f;
						this->p_isGrounded = true;
						this->p_isJumping = false;
					}
					//if player is under collidable
					else
					{
						tmpPos.y += overlapY;
						this->UpdatePosition(tmpPos);

						if (this->p_velocityVector.y > 0.f)
							this->p_velocityVector.y = 0.f;
					}
					break;
				case Player::GravityDirection::Down:
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
					break;
				case Player::GravityDirection::Left:
				case Player::GravityDirection::Right:
					if (this->GetCollider().p_maxPoint.y > obj->GetCollider().p_minPoint.y &&
						this->GetCollider().p_minPoint.y < obj->GetCollider().p_minPoint.y)
					{
						// Player hit OBJECT BOTTOM side
						std::cout << "left hit" << std::endl;
						tmpPos.y -= overlapY;
						this->canMoveLeft = false;
					}
					else
					{
						// Player hit OBJECT TOP side
						std::cout << "right hit" << std::endl;
						tmpPos.y += overlapY;
						this->canMoveRight = false;
					}
					this->UpdatePosition(tmpPos);
					this->p_velocityVector.y = 0.f;
					break;
				default:
					break;
			}
		}
		else
		{
			//temp side collision
			AEVec2 tmpPos = this->GetPosition();

			switch (dir) {
				//this two cases is for normal vertical check. when X offset is smaller than Y (push X)
				case Player::GravityDirection::Up:
				case Player::GravityDirection::Down:
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
						tmpPos.x += overlapX;
						this->canMoveLeft = false;
					}
					this->UpdatePosition(tmpPos);
					this->p_velocityVector.x = 0.f;
					break;

				case Player::GravityDirection::Left:
					if (this->GetPosition().x > obj->GetPosition().x)
					{
						tmpPos.x += overlapX;
						this->UpdatePosition(tmpPos);

						this->p_velocityVector.x = 0.f;
						this->p_isGrounded = true;
						this->p_isJumping = false;
					}
					//if player is under collidable
					else
					{
						tmpPos.x -= overlapX;
						this->UpdatePosition(tmpPos);

						if (this->p_velocityVector.x > 0.f)
							this->p_velocityVector.x = 0.f;
					}
					break;
				case Player::GravityDirection::Right:
					if (this->GetPosition().x < obj->GetPosition().x)
					{
						tmpPos.x -= overlapX;
						this->UpdatePosition(tmpPos);

						this->p_velocityVector.x = 0.f;
						this->p_isGrounded = true;
						this->p_isJumping = false;
					}
					//if player is under collidable
					else
					{
						tmpPos.x += overlapX;
						this->UpdatePosition(tmpPos);

						if (this->p_velocityVector.x < 0.f)
							this->p_velocityVector.x = 0.f;
					}
					break;
				default:
					break;
			}
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

