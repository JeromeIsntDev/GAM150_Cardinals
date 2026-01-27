/*!***********************************************************************
\file	Physics.cpp
\author	Sadric Koh
\date	27/01/2026
\par	Last Updated by: Sadric
\par	Project: Cardinals

\brief
holds the class Player which contains player only variables and inherits
from GameObject class from GameObject


\par	Changes: Added Player initializer
**************************************************************************/

#include "../header/Physics.hpp"



void Collider::InitializeCollider(AEVec2 centrePoint, f32 xLength, f32 yLength) {
	//X points
	p_minPoint.x = centrePoint.x - (yLength / 2.f) - (xLength / 2.f); //bottom left
	p_maxPoint.x = centrePoint.x + (yLength / 2.f) - (xLength / 2.f); //bottom right
	//Y points
	p_minPoint.y = centrePoint.y + (yLength / 2.f) - (xLength / 2.f); //upper left
	p_maxPoint.y = centrePoint.y + (yLength / 2.f) + (xLength / 2.f); //upper right
}

bool IsCollidedGround() {
	//check bottom Y position of object with ground
	return true;
}

bool IsCollidedWall() {
	//check left right with wall
	return true;\
}
