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
f32 clampedDT{};

void Player::InitializePlayer(int health, f32 moveSpeed, bool isJump, f32 score, f32 zeroVal, AEGfxVertexList* newMesh, f32 xPos, f32 yPos, bool gOver) {
	p_playerHealth = health;
	p_moveSpeed = moveSpeed;
	p_isJumping = isJump;
	playerScore = score;
	p_currPos.x = xPos;
	p_currPos.y = yPos;
	UpdatePosition({xPos, yPos});
	UpdatePosition(p_currPos);
	UpdateRotation(0.f);
	p_velocityVector.x = zeroVal;
	p_velocityVector.y = zeroVal;
	p_Scale.y = 50.f;
	Player::mesh = newMesh;
	p_previousPos = p_currPos;
	grav = GravityDirection::Down;
	gameOver = gOver;
	p_isGrounded = true;
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

Player::Player()
	: GameObject()
{
}

void Player::PlayerInputHandle(f32 const& dt, GravityDirection& dir) {
	f32 moveSpeed{ 300.f };
	isMoving = false;
	isSprint = false;

	switch (dir) {
		case GravityDirection::Left:
		case GravityDirection::Right:
			if (canMoveRight) {
				if (input.isKeyPressed(AEVK_S)) {
					p_velocityVector.y = -1.f * p_moveSpeed * dt;
					p_currPos.y += p_velocityVector.y;
					p_Scale.x = 50.f;
				}
			}
			if (canMoveLeft) {
				if (input.isKeyPressed(AEVK_W)) {
					p_velocityVector.y = 1.f * p_moveSpeed * dt;
					p_currPos.y += p_velocityVector.y;
					p_Scale.x = -50.f;
				}
			}
			isMoving = true;
			break;
		case GravityDirection::Up:
		case GravityDirection::Down:
			if (canMoveRight) {
				if (input.isKeyPressed(AEVK_D)) {
					p_velocityVector.x = 1.f * p_moveSpeed * dt;
					p_currPos.x += p_velocityVector.x;
					p_velocityVector.x = 0.f;
					p_Scale.x = 50.f;
				}
			}
			if (canMoveLeft) {
				if (input.isKeyPressed(AEVK_A)) {
					p_velocityVector.x = -1.f * p_moveSpeed * dt;
					p_currPos.x += p_velocityVector.x;
					p_velocityVector.x = 0.f;
					p_Scale.x = -50.f;
				}
			}
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
		p_moveSpeed = maxSpeed;
	}
	else {
		p_moveSpeed = moveSpeed;
	}
	//std::cout << "left bool: " << canMoveLeft << std::endl;
	//std::cout << "right bool: " << canMoveRight << std::endl;

	if (input.isKeyTriggered(AEVK_SPACE) && !p_isJumping) {
		bgm.PlayAudio("JUMP", AudioType::SFX, 1.f, 0);
		//adjust jump logic here
		switch(dir){
		case Player::GravityDirection::Up:
			p_velocityVector.y = -jumpForce;
			p_velocityVector.x = 0.f;
			break;
		case Player::GravityDirection::Down:
			p_velocityVector.y = jumpForce;
			p_velocityVector.x = 0.f;
			break;
		case Player::GravityDirection::Left:
			p_velocityVector.x = jumpForce;
			p_velocityVector.y = 0.f;
			break;
		case Player::GravityDirection::Right:
			p_velocityVector.x = -jumpForce;
			p_velocityVector.y = 0.f;
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
	//down force velocity (always going downwards) depend on location

	if (hitCD > 0.f) {
		hitCD -= dt;
	}

	if (hitAnim) {
		rot_Timer += dt;
		while (rot_Timer >= .045f && rot_count < 12) {
			UpdateRotation(this->GetRotation() + (PI / 4.f));
			//std::cout << "this is running" << std::endl;
			rot_Timer -= .045f;
			++rot_count;
		}

		if (rot_count >= 12) {
			UpdateRotation(0.f);
			isHit = false;
			//std::cout << "this ran" << std::endl;
			--p_playerHealth;

			rot_Timer = 0.0f;
			rot_count = 0;
			hitAnim = false;

			p_currPos = p_previousPos;
			canMove = true;
			grav = GravityDirection::Down;
			p_velocityVector = { 0.f, 0.f };
			p_Scale = { 50.f, 50.f };
		}
		if (p_playerHealth <= 0) {
			bgm.StopAudio();
			bgm.PlayAudio("FAILOVERLAY", AudioType::MUSIC, 1.f, -1);
			p_velocityVector = { 0.f, 0.f };
			p_Scale = { 50.f,50.f };
			grav = GravityDirection::Down;
			gameOver = true;
		}
	}

	if (!p_isGrounded) {
		clampedDT = AEClamp(clampedDT,dt, 0.01667f);
		switch (dir)
		{
		case Player::GravityDirection::Up:
			if (!isHit) {
				p_velocityVector.y += (gravity * p_downForce) * clampedDT;
			}
			break;
		case Player::GravityDirection::Down:
			if (!isHit) {
				p_velocityVector.y += (-gravity * p_downForce) * clampedDT;
			}
			break;
		case Player::GravityDirection::Left:
			if (!isHit) {
				p_velocityVector.x += (-gravity * p_downForce) * clampedDT;
			}
			break;
		case Player::GravityDirection::Right:
			if (!isHit) {
				p_velocityVector.x += (gravity * p_downForce) * clampedDT;
			}
			break;
		default:
			break;
		}
		//CLAMP HERE
		p_velocityVector.y = AEClamp(p_velocityVector.y, -550.f, 550.f);
		p_velocityVector.x = AEClamp(p_velocityVector.x, -500.f, 500.f);
		if (!isHit) {
			p_currPos.x += (p_velocityVector.x) * dt;
			p_currPos.y += (p_velocityVector.y) * dt;
		}
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
	//std::cout << p_currPos.y << std::endl;
	if (p_currPos.y <= -500.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		UpdateRotation(0.f);
		grav = Player::GravityDirection::Down;
	}
	if (p_currPos.y >= 400.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		UpdateRotation(0.f);
		grav = Player::GravityDirection::Down;
	}
	if (p_currPos.x >= 750.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		UpdateRotation(0.f);
		grav = Player::GravityDirection::Down;
	}
	if (p_currPos.x <= -750.f) {
		p_currPos = p_previousPos;
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		p_playerHealth--;
		UpdateRotation(0.f);
		grav = Player::GravityDirection::Down;
	}

	if (p_playerHealth <= 0) {
		p_velocityVector = { 0.f, 0.f };
		p_Scale = { 50.f,50.f };
		grav = Player::GravityDirection::Down;
		gameOver = true;
	}
	if (canMove) {
		UpdatePosition(p_currPos);
	}
}

void Player::CheckBinaryCollision(Grid* grid, GravityDirection& dir) {
	int collideVal = CheckInstanceBinaryMapCollision(GetPosition().x,
		GetPosition().y, GetScale().x, GetScale().y, grid);

	gridCollisionFlag = collideVal;

	//std::cout << p_currPos.y << " " << bottom << std::endl;

	switch (dir) {
		case GravityDirection::Down:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				p_velocityVector.y = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				p_velocityVector.y = 0.f;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				canMoveLeft = false;
				p_velocityVector.x = 0.f;
				grid->SnapToCell(&p_currPos.x);
				//PRINT("DEBUG LEFT");
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				canMoveRight = false;
				p_velocityVector.x = 0.f;
				grid->SnapToCell(&p_currPos.x);
			}
		break;
		case GravityDirection::Up:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				p_velocityVector.y = 0.f;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				p_velocityVector.y = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				canMoveLeft = false;
				p_velocityVector.x = 0.f;
				grid->SnapToCell(&p_currPos.x);
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				canMoveRight = false;
				p_velocityVector.x = 0.f;
				grid->SnapToCell(&p_currPos.x);
			}
		break;
		case GravityDirection::Left:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				canMoveRight = false;
				p_velocityVector.y = 0.f;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				p_velocityVector.y = 0.f;
				canMoveLeft = false;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				p_velocityVector.x = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
				grid->SnapToCell(&p_currPos.x);
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				p_velocityVector.x = 0.f;
				grid->SnapToCell(&p_currPos.x);
			}
		break;
		case GravityDirection::Right:
			if (gridCollisionFlag & COLLISION_BOTTOM) {
				canMoveRight = false;
				p_velocityVector.y = 0.f;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_TOP) {
				canMoveLeft = false;
				p_velocityVector.y = 0.f;
				grid->SnapToCell(&p_currPos.y);
			}
			if (gridCollisionFlag & COLLISION_LEFT) {
				p_velocityVector.x = 0.f;
				grid->SnapToCell(&p_currPos.x);
			}
			if (gridCollisionFlag & COLLISION_RIGHT) {
				p_velocityVector.x = 0.f;
				p_isGrounded = true;
				p_isJumping = false;
				grid->SnapToCell(&p_currPos.x);
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
	y1 = PosY + quarterScaleY;
	x2 = PosX + halfScaleX;
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
			Flag += COLLISION_RIGHT;
		}
	}

	x1 = PosX - halfScaleX - .2f; //Added for some padding due to no conversion of world to grid initially
	y1 = PosY + quarterScaleY;
	x2 = PosX - halfScaleX;
	y2 = PosY - quarterScaleY;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static ||
			cell2->tag == GameObjectTag::Static) {
			Flag += COLLISION_LEFT;
		}
	}

	x1 = PosX + quarterScaleX;
	y1 = PosY + halfScaleY + .1f; //Added for some padding due to no conversion of world to grid initially
	x2 = PosX - quarterScaleX;
	y2 = PosY + halfScaleY;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static ||
			cell2->tag == GameObjectTag::Static) {
			Flag += COLLISION_TOP;
		}
	}

	x1 = PosX + quarterScaleX;
	y1 = PosY - halfScaleY;
	x2 = PosX - quarterScaleX;
	y2 = PosY - halfScaleY;

	grid->WorldToGrid(&x1, &y1);
	grid->WorldToGrid(&x2, &y2);
	cell1 = grid->GetCell(y1, x1);
	cell2 = grid->GetCell(y2, x2);
	if (cell1 && cell2)
	{
		if (cell1->tag == GameObjectTag::Static || 
			cell2->tag == GameObjectTag::Static) {
			Flag += COLLISION_BOTTOM;
		}
	}

	return Flag;
}

void Player::PlayerCollision(std::vector<GameObject*> objs, f32& dt)
{
	for (GameObject* obj : objs) {
		if (this->_tag == obj->_tag) continue;
		if (!CollisionDetected(this->GetCollider(), obj->GetCollider())) continue;

		AEVec2 currentScale = obj->GetScale();
		AEVec2 currentPos = obj->GetPosition();

		this->hasCollided = true;


		if (obj->_tag == GameObjectTag::Win) {
			//win gate hit
			bgm.StopAudio();
			bgm.PlayAudio("WINOVERLAY", AudioType::MUSIC, 1.f, -1);
			std::cout << "Win gate hit" << std::endl;
			p_velocityVector = { 0.f, 0.f };
			p_Scale = { 50.f,50.f };
			UpdateRotation(0.f);
			gameOver = true;
		}
		else if (obj->_tag == GameObjectTag::GravUp) {
			if (!audioPlayed) {
				bgm.PlayAudio("GRAVITY", AudioType::SFX, 1.f, 0);
				audioPlayed = true;
			}
			std::cout << "grav up" << std::endl;
			grav = Player::GravityDirection::Up;
			UpdateRotation(0.f);
			p_Scale.y = -50.f;
		}
		else if (obj->_tag == GameObjectTag::GravDown) {
			if (!audioPlayed) {
				bgm.PlayAudio("GRAVITY", AudioType::SFX, 1.f, 0);
				audioPlayed = true;
			}
			grav = Player::GravityDirection::Down;
			UpdateRotation(0.f);
			p_Scale.y = 50.f;
		}
		else if (obj->_tag == GameObjectTag::GravLeft) {
			if (!audioPlayed) {
				bgm.PlayAudio("GRAVITY", AudioType::SFX, 1.f, 0);
				audioPlayed = true;
			}
			grav = Player::GravityDirection::Left;
			p_Scale.y = 50.f;
			UpdateRotation(-(PI / 2.f));
		}
		else if (obj->_tag == GameObjectTag::Gravright) {
			if (!audioPlayed) {
				bgm.PlayAudio("GRAVITY", AudioType::SFX, 1.f, 0);
				audioPlayed = true;
			}
			grav = Player::GravityDirection::Right;
			UpdateRotation(-(PI / 2.f));
			p_Scale.y = -50.f;
		}
		else if (obj->_tag == GameObjectTag::Obstacle && !isHit) {
			if (!hitAnim && p_playerHealth > 0 && hitCD <= 0.f) {
				isHit = true;
				std::cout << "obstacle hit" << std::endl;
				hitAnim = true;
				canMove = false;
				bgm.PlayAudio("HIT", AudioType::SFX, 1.f, 0);
				rot_Timer = 0.f;
				rot_count = 0.f;
				hitCD = 0.2f;
			}
		}
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

AEVec2 Player::GetVelocity() const {
	return p_velocityVector;
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
