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
	f32 halfWidth = xLength * 0.5f;
	f32 halfHeight = yLength * 0.5f;
	//minPoint
	p_minPoint = { centrePoint.x - halfHeight - halfWidth, centrePoint.y - halfWidth - halfHeight };
	//maxPoint
	p_maxPoint = { centrePoint.x + halfWidth + halfHeight, centrePoint.y + halfHeight + halfWidth };
}

//collider logic
bool Collider::CollisionDetected(const Collider& collider1, const Collider& collider2) {
	return !(collider1.p_maxPoint.x < collider2.p_minPoint.x ||
			 collider1.p_minPoint.x > collider2.p_maxPoint.x ||
			 collider1.p_maxPoint.y < collider2.p_minPoint.y ||
			 collider1.p_minPoint.y > collider2.p_maxPoint.y);
}