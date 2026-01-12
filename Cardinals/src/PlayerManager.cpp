/*!***********************************************************************
\file	PlayerManager.cpp
\author	Sadric Koh
\date	12/01/2026
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief	The Manager that holds everything related to player

\par	Changes: None
**************************************************************************/

#include "../header/PlayerManager.hpp"

//takes in the current player pos, deltaTime & movement vector to update the player pos
void Player::UpdatePlayerPos(AEVec2 pos, f32 deltaTime, AEVec2 movementVec)
{
	pos.x += movementVec.x * moveSpeed * deltaTime;
	//possible change to this Y movement
	if (AEInputCheckTriggered(AEVK_SPACE) && canJump) {
		pos.y += movementVec.y * moveSpeed * deltaTime;
	}

	GameObject::UpdatePosition(pos);
}
