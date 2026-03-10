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
#include "../header/GStateManager.hpp"
#include <iostream>

//Player::Player(f32 health ,f32 moveSpeed,f32 jumpForce ,bool isJump ,f32 score , f32 zeroVal, AEGfxVertexList* mesh, f32 xPos, f32 yPos) 
//	: p_playerHealth(health), p_moveSpeed(moveSpeed), p_jumpForce(jumpForce),
//	  p_isJumping(isJump),p_score(score),p_currPos{xPos,yPos},
//	  initialYPos(zeroVal),initialVelocity(zeroVal),jumpT(zeroVal),
//	  GameObject(mesh,AEVec2{0.f,0.f},0.f,AEVec2{200.f,200.f})
//{}

auto& input = InputHandler::GetInstance();

void Player::InitializePlayer(int health, f32 moveSpeed, bool isJump, f32 score, f32 zeroVal, AEGfxVertexList* newMesh, f32 xPos, f32 yPos) {
	p_playerHealth = health;
	p_moveSpeed = moveSpeed;
	p_isJumping = isJump;
	playerScore = score;
	p_currPos.x = xPos;
	p_currPos.y = yPos;
	UpdatePosition(p_currPos);
	p_velocityVector.y = zeroVal;
	Player::mesh = newMesh;
	p_previousPos = p_currPos;
	grav = GravityDirection::Down;
}

//takes in the current player pos, deltaTime & movement vector to update the player pos
//void Player::UpdatePlayerPos(AEVec2 pos, AEVec2 movementVec, AEVec2 velocityVec)
//{
//	//input handling
//	GameObject::UpdatePosition(pos);
//}

//Takes in the initial value of points for the code to reference during jump state
//void Player::StartJump(AEVec2 velocityVec) {
//	if (p_isJumping) return;
//
//}
//Math logic behind jump for parabola jump
void Player::PlayerJump(f32 const& dt, GravityDirection& dir) {
	if (!p_isJumping) return;
	switch (dir) {
	case GravityDirection::Up:
	case GravityDirection::Down:
		p_velocityVector.y += gravity * dt;
		p_currPos.y += p_velocityVector.y * dt;
		break;
	case GravityDirection::Left:
	case GravityDirection::Right:
		p_velocityVector.x += gravity * dt;
		p_currPos.x += p_velocityVector.x * dt;
		break;
	}
	
	jumpBuffer = 0.f;
	coyoteBuffer = 0.f;
}

void Player::PlayerInputHandle(f32 const& dt, GravityDirection& dir) {
	f32 moveSpeed{ 300.f };
	bool isMoving{ false };
	bool isSprint{ false };
	if (input.isKeyPressed(AEVK_D)) {
		if (canMoveRight) {
			switch (dir) {
			case GravityDirection::Up:
			case GravityDirection::Down:
				p_velocityVector.x = 1.f * p_moveSpeed * dt;
				p_currPos.x += p_velocityVector.x;
				p_velocityVector.x = 0.f;
				break;
			case GravityDirection::Left:
			case GravityDirection::Right:
				p_velocityVector.y = -1.f * p_moveSpeed * dt;
				p_currPos.y += p_velocityVector.y;
				break;
			default:
				break;
			}
		}
		p_Scale.x = 50.f;
		isMoving = true;
	}
	if (input.isKeyPressed(AEVK_A)) {
		//p_velocityVector.x = 0.f;
		if (canMoveLeft) {
			switch (dir) {
			case GravityDirection::Up:
			case GravityDirection::Down:
				p_velocityVector.x = -1.f * p_moveSpeed * dt;
				p_currPos.x += p_velocityVector.x;
				p_velocityVector.x = 0.f;
				break;
			case GravityDirection::Left:
			case GravityDirection::Right:
				p_velocityVector.y = 1.f * p_moveSpeed * dt;
				p_currPos.y += p_velocityVector.y;
				break;
			default:
				break;
			}
		}
		p_Scale.x = -50.f;
		isMoving = true;
	}
	if (input.isKeyPressed(AEVK_LSHIFT) && isMoving) {
		isSprint = true;
	}
	if (input.isKeyReleased(AEVK_LSHIFT)) {
		isSprint = false;
	}
	GravityDirection currDir = dir;
	changeCD += dt;
	if (currDir != prevDir)
	{
		canSwapGrav = false;
		if (changeCD >= maxCD)
		{
			//std::cout << "this ran" << std::endl;
			canSwapGrav = true;
			changeCD = 0.f;
			prevDir = currDir;
		}
	}
	//Sprint speed logic
	if (isSprint) {
		f32 maxSpeed = moveSpeed * 1.2f;
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
	//std::cout << "left bool: " << canMoveLeft << std::endl;
	//std::cout << "right bool: " << canMoveRight << std::endl;

	if (input.isKeyPressed(AEVK_SPACE) && !p_isJumping) {
		bgm.PlayAudio("JUMP", AudioType::SFX, 1.f, 0);
		//adjust jump logic here
		switch(dir){
		case Player::GravityDirection::Up:
			p_velocityVector.y = -jumpForce;
			break;
		case Player::GravityDirection::Down:
			p_velocityVector.y = jumpForce;
			break;
		case Player::GravityDirection::Left:
			p_velocityVector.x = jumpForce;
			break;
		case Player::GravityDirection::Right:
			p_velocityVector.x = -jumpForce;
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
	UpdateScale(p_Scale);
}

//Player Movement Logic
void Player::PlayerMovement(f32 const& dt, GravityDirection& dir) {
	//std::cout << "dt: " << dt << std::endl;

	jumpBuffer = max(0, jumpBuffer - dt);
	coyoteBuffer = p_isGrounded ? maxCoyoteBuffer : max(0, coyoteBuffer - dt);
	GravityDirection currDir = dir;
	//std::cout << p_currPos.y << std::endl;
	//down force velocity (always going downwards) depend on location
	if (!p_isGrounded) {
		switch (dir)
		{
		case Player::GravityDirection::Up:
			p_velocityVector.y += (gravity * p_downForce) * dt;
			p_currPos.y += (p_velocityVector.y) * dt;
			p_currPos.x += (p_velocityVector.x) * dt;
			break;
		case Player::GravityDirection::Down:
			p_velocityVector.y += (-gravity * p_downForce) * dt;
			p_currPos.y += (p_velocityVector.y) * dt;
			p_currPos.x += (p_velocityVector.x) * dt;
			break;
		case Player::GravityDirection::Left:
			p_velocityVector.x += (-gravity * p_downForce) * dt;
			p_currPos.x += (p_velocityVector.x) * dt;
			p_currPos.y += (p_velocityVector.y) * dt;
			break;
		case Player::GravityDirection::Right:
			p_velocityVector.x += (gravity * p_downForce) * dt;
			p_currPos.x += (p_velocityVector.x) * dt;
			p_currPos.y += (p_velocityVector.y) * dt;
			break;
		default:
			break;
		}
		//CLAMP HERE
		p_velocityVector.y = AEClamp(p_velocityVector.y, -500.f, 500.f);
		p_velocityVector.x = AEClamp(p_velocityVector.x, -500.f, 500.f);
		/*p_velocityVector.y += gravity * dt;
		p_currPos.y += (p_velocityVector.y * p_downForce) * dt;*/
	}
	//std::cout << "X: " << p_currPos.x << "Y: " << p_currPos.y << std::endl;
	//give a cool down to prevent being able to jump instantly
	//std::cout << "timer: " << currCD << std::endl;
	currCD += dt;
	if (currDir != prevDir)
	{
		p_isJumping = true;
		if (currCD >= maxCD)
		{
			//std::cout << "this ran" << std::endl;
			p_isJumping = false;
			currCD = 0.f;
			prevDir = currDir;
		}
	}

	PlayerInputHandle(dt, dir);
	//if top of player goes past bottom of screen (temporary stuff, probably wont need)
	if (p_currPos.y + (GetScale().y * 0.5f) <= -AEGfxGetWindowHeight() + AEGfxGetWinMaxY() + (GetScale().y / 2.f)) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		grav = Player::GravityDirection::Down;
	}
	if (p_currPos.y >= 400.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		grav = Player::GravityDirection::Down;
	}
	if (p_currPos.x >= 750.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		grav = Player::GravityDirection::Down;
	}
	if (p_currPos.x <= -750.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		grav = Player::GravityDirection::Down;
	}

	if (p_playerHealth <= 0) {
		gameOver = true;
	}

	//PRINT("Momentum: %f,%f", p_velocityVector.x, p_velocityVector.y);

	UpdatePosition(p_currPos);
}

void Player::CheckBinaryCollision(Grid* grid, GravityDirection& dir) {
	int collideVal = CheckInstanceBinaryMapCollision(GetPosition().x,
		GetPosition().y, GetScale().x, GetScale().y, grid);

	gridCollisionFlag = collideVal;

	f32 halfScaleY = p_Scale.y * 0.5f;
	f32 halfScaleX = p_Scale.x * 0.5f;
	f32 bottom = p_currPos.y - halfScaleY;
	f32 top = p_currPos.y + halfScaleY;
	f32 left = p_currPos.x - halfScaleX;
	f32 right = p_currPos.x + halfScaleX;

	//std::cout << p_currPos.y << " " << bottom << std::endl;

	switch (dir) {
		case GravityDirection::Down:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				p_velocityVector.y = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				p_velocityVector.y = 0.f;
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				canMoveLeft = false;
				p_velocityVector.x = 0.f;
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				canMoveRight = false;
				p_velocityVector.x = 0.f;
			}
		break;
		case GravityDirection::Up:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				p_velocityVector.y = 0.f;
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				p_velocityVector.y = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				canMoveLeft = false;
				p_velocityVector.x = 0.f;
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				canMoveRight = false;
				p_velocityVector.x = 0.f;
			}
		break;
		case GravityDirection::Left:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				canMoveRight = false;
				p_velocityVector.y = 0.f;
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				p_velocityVector.y = 0.f;
				canMoveLeft = false;
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				p_velocityVector.x = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				p_velocityVector.x = 0.f;
			}
		break;
		case GravityDirection::Right:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				canMoveRight = false;
				p_velocityVector.y = 0.f;
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				canMoveLeft = false;
				p_velocityVector.y = 0.f;
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				p_velocityVector.x = 0.f;
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				p_velocityVector.x = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
			}
		break;
	}
}

int Player::CheckInstanceBinaryMapCollision(f32 PosX, f32 PosY, f32 scaleX, f32 scaleY, Grid* grid) {
	int Flag = 0;
	//right side
	f32 x1, y1, x2, y2;
	f32 halfScaleX = fabsf(scaleX * .5f);
	f32 halfScaleY = fabsf(scaleY * .5f);
	f32 quarterScaleX = fabsf(scaleX * .25f);
	f32 quarterScaleY = fabsf(scaleY * .25f);

	GridCell* cell1, *cell2;

	x1 = PosX + halfScaleX;
	//grid->SnapToCell(&r_x1);
	y1 = PosY + quarterScaleY;
	//grid->SnapToCell(&r_y1);
	x2 = PosX + halfScaleX;
	//grid->SnapToCell(&r_x2);
	y2 = PosY - quarterScaleY;

	if (!grid) return Flag;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static ||
			cell2->tag == GameObjectTag::Static) {
			//PRINT("[DEBUG] => COLLIDE RIGHT\n");
			Flag += COLLISION_RIGHT;
		}
	}

	x1 = PosX - halfScaleX;
	//grid->SnapToCell(&r_x1);
	y1 = PosY + quarterScaleY;
	//grid->SnapToCell(&r_y1);
	x2 = PosX - halfScaleX;
	//grid->SnapToCell(&r_x2);
	y2 = PosY - quarterScaleY;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static ||
			cell2->tag == GameObjectTag::Static) {
			//PRINT("[DEBUG] => COLLIDE LEFT\n");
			Flag += COLLISION_LEFT;
		}
	}

	x1 = PosX + quarterScaleX;
	//grid->SnapToCell(&r_x1);
	y1 = PosY + halfScaleY;
	//grid->SnapToCell(&r_y1);
	x2 = PosX - quarterScaleX;
	//grid->SnapToCell(&r_x2);
	y2 = PosY + halfScaleY;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static ||
			cell2->tag == GameObjectTag::Static) {
			//PRINT("[DEBUG] => COLLIDE TOP\n");
			Flag += COLLISION_TOP;
		}
	}

	x1 = PosX + quarterScaleX;
	//grid->SnapToCell(&r_x1);
	y1 = PosY - halfScaleY;
	//grid->SnapToCell(&r_y1);
	x2 = PosX - quarterScaleX;
	//grid->SnapToCell(&r_x2);
	y2 = PosY - halfScaleY;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static || 
			cell2->tag == GameObjectTag::Static)
		{
			//PRINT("[DEBUG] => COLLIDE BOTTOM\n");
			Flag += COLLISION_BOTTOM;
		}
	}

	f32 playerX{ PosX }, playerY{ PosY };
	grid->WorldToGrid(&playerX, &playerY);

	return Flag;
}

void Player::PlayerCollision(std::vector<GameObject*> objs, GravityDirection& dir)
{
	for (GameObject* obj : objs) {
		if (this->_tag == obj->_tag) continue;
		if (!CollisionDetected(this->GetCollider(), obj->GetCollider())) continue;

		AEVec2 currentScale = obj->GetScale();
		AEVec2 currentPos = obj->GetPosition();

		this->hasCollided = true;


		if (obj->_tag == GameObjectTag::Win) {
			//win gate hit
			bgm.PlayAudio("HITWIN", AudioType::SFX, 1.f, 0);
			std::cout << "Win gate hit" << std::endl;
			p_velocityVector = { 0.f,0.f };
			gameOver = true;
		}
		else if (obj->_tag == GameObjectTag::GravUp) {
			std::cout << "grav up" << std::endl;
			grav = Player::GravityDirection::Up;
			p_Scale.y = -50.f;
		}
		else if (obj->_tag == GameObjectTag::GravDown) {
			grav = Player::GravityDirection::Down;
			p_Scale.y = 50.f;
		}
		else if (obj->_tag == GameObjectTag::GravLeft) {
			grav = Player::GravityDirection::Left;
		}
		else if (obj->_tag == GameObjectTag::Gravright) {
			grav = Player::GravityDirection::Right;
		}
		else if (obj->_tag == GameObjectTag::Obstacle) {
			std::cout << "obstacle hit" << std::endl;
			p_playerHealth--;
			if (p_playerHealth <= 0) {
				gameOver = true;
			}
			else {
				p_currPos = p_previousPos;
				grav = GravityDirection::Down;
			}
		}
		/*else {
		//	f32 overlapX =
		//		(std::min)(this->GetCollider().p_maxPoint.x, obj->GetCollider().p_maxPoint.x) -
		//		(std::max)(this->GetCollider().p_minPoint.x, obj->GetCollider().p_minPoint.x);

		//	f32 overlapY =
		//		(std::min)(this->GetCollider().p_maxPoint.y, obj->GetCollider().p_maxPoint.y) -
		//		(std::max)(this->GetCollider().p_minPoint.y, obj->GetCollider().p_minPoint.y);

		//	if (overlapX <= 0.f || overlapY <= 0.f) continue;

		//	// Resolve on smallest penetration axis (Y axis)
		//	if (overlapY <= overlapX + 5.f)
		//	{
		//		AEVec2 tmpPos = this->GetPosition();
		//		switch (dir) {
		//		case Player::GravityDirection::Up:
		//			if (this->GetCollider().p_maxPoint.y > obj->GetCollider().p_minPoint.y &&
		//				this->GetCollider().p_maxPoint.y < obj->GetCollider().p_maxPoint.y)
		//			{
		//				tmpPos.y -= overlapY;
		//				this->UpdatePosition(tmpPos);

		//				if (this->p_velocityVector.y > 0.f)
		//					this->p_velocityVector.y = 0.f;
		//				this->p_isGrounded = true;
		//				this->p_isJumping = false;
		//			}
		//			//if player is under collidable
		//			else if (this->GetCollider().p_maxPoint.y > obj->GetCollider().p_minPoint.y &&
		//					 this->GetCollider().p_maxPoint.y < obj->GetCollider().p_maxPoint.y)
		//			{
		//				tmpPos.y += overlapY;
		//				this->UpdatePosition(tmpPos);

		//				if (this->p_velocityVector.y < 0.f)
		//					this->p_velocityVector.y = 0.f;
		//			}
		//			break;
		//		case Player::GravityDirection::Down:
		//			//if player is above collidable
		//			if (this->GetCollider().p_minPoint.y < obj->GetCollider().p_maxPoint.y &&
		//				this->GetCollider().p_maxPoint.y > obj->GetCollider().p_maxPoint.y)
		//			{
		//				if (this->p_velocityVector.y < 0.f)
		//					this->p_velocityVector.y = 0.f;
		//				tmpPos.y += overlapY;
		//				this->UpdatePosition(tmpPos);
		//				this->p_isGrounded = true;
		//				this->p_isJumping = false;
		//			}
		//			//if player is under collidable
		//			else if (this->GetCollider().p_maxPoint.y > obj->GetCollider().p_minPoint.y &&
		//					 this->GetCollider().p_maxPoint.y < obj->GetCollider().p_maxPoint.y)
		//			{
		//				if (this->p_velocityVector.y > 0.f)
		//					this->p_velocityVector.y = 0.f;
		//				tmpPos.y -= overlapY;
		//				this->UpdatePosition(tmpPos);

		//			}
		//			break;
		//		case Player::GravityDirection::Left:
		//		case Player::GravityDirection::Right:
		//			if (this->GetCollider().p_maxPoint.y > obj->GetCollider().p_minPoint.y &&
		//				this->GetCollider().p_maxPoint.y < obj->GetCollider().p_maxPoint.y)
		//			{
		//				// Player hit OBJECT BOTTOM side
		//				std::cout << "left hit" << std::endl;
		//				tmpPos.y -= overlapY;
		//				this->canMoveLeft = false;
		//			}
		//			else if (this->GetCollider().p_minPoint.y < obj->GetCollider().p_maxPoint.y &&
		//					 this->GetCollider().p_minPoint.y > obj->GetCollider().p_minPoint.y)
		//			{
		//				// Player hit OBJECT TOP side
		//				std::cout << "right hit" << std::endl;
		//				tmpPos.y += overlapY;
		//				this->canMoveRight = false;
		//			}
		//			this->UpdatePosition(tmpPos);
		//			if (this->p_velocityVector.y > 0.f)
		//				this->p_velocityVector.y = 0.f;
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		//temp side collision
		//		AEVec2 tmpPos = this->GetPosition();

		//		switch (dir) {
		//			//this two cases is for normal vertical check. when X offset is smaller than Y (push X)
		//		case Player::GravityDirection::Up:
		//		case Player::GravityDirection::Down:
		//			if (this->GetCollider().p_maxPoint.x > obj->GetCollider().p_minPoint.x &&
		//				this->GetCollider().p_maxPoint.x < obj->GetCollider().p_maxPoint.x)
		//			{
		//				// Player hit object's LEFT side
		//				tmpPos.x -= overlapX; 
		//				this->canMoveRight = false;
		//			}
		//			else if (this->GetCollider().p_minPoint.x < obj->GetCollider().p_maxPoint.x &&
		//					 this->GetCollider().p_minPoint.x > obj->GetCollider().p_minPoint.x)
		//			{
		//				// Player hit object's RIGHT side
		//				tmpPos.x += overlapX;
		//				this->canMoveLeft = false;
		//			}
		//			this->UpdatePosition(tmpPos);
		//			this->p_velocityVector.x = 0.f;
		//			break;
		//		case Player::GravityDirection::Left:
		//			if (this->GetPosition().x > obj->GetPosition().x)
		//			{
		//				tmpPos.x += overlapX;
		//				this->UpdatePosition(tmpPos);

		//				this->p_velocityVector.x = 0.f;
		//				this->p_isGrounded = true;
		//				this->p_isJumping = false;
		//			}
		//			//if player is under collidable
		//			else
		//			{
		//				tmpPos.x -= overlapX;
		//				this->UpdatePosition(tmpPos);

		//				if (this->p_velocityVector.x > 0.f)
		//					this->p_velocityVector.x = 0.f;
		//			}
		//			break;
		//		case Player::GravityDirection::Right:
		//			if (this->GetPosition().x < obj->GetPosition().x)
		//			{
		//				tmpPos.x -= overlapX;
		//				this->UpdatePosition(tmpPos);

		//				this->p_velocityVector.x = 0.f;
		//				this->p_isGrounded = true;
		//				this->p_isJumping = false;
		//			}
		//			//if player is under collidable
		//			else
		//			{
		//				tmpPos.x += overlapX;
		//				this->UpdatePosition(tmpPos);

		//				if (this->p_velocityVector.x < 0.f)
		//					this->p_velocityVector.x = 0.f;
		//			}
		//			break;
		//		default:
		//			break;
		//		}
		//	}
		//}*/
	}
}

//sets the player health to smth new depending on parameter
//void Player::SetPlayerHealth(f32 healthDamage) {
//	p_playerHealth -= healthDamage;
//}

bool Player::GetPlayerCollide() const{
	return hasCollided;
}

bool Player::getGameOver() const {
	return gameOver;
}

void Player::SetPlayerCollide(bool collide) {
	hasCollided = collide;
}

void Player::SetGameOver(bool state) {
	gameOver = state;
}

void Player::SetHealth(int hp) {
	p_playerHealth = hp;
}

AEVec2 Player::GetPrevPos() const {
	return p_previousPos;
}

int Player::GetPlayerHealth() const{
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
