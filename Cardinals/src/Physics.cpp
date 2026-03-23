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
#include <iostream>


void Collider::UpdateCollider(AEVec2 centrePoint, f32 xLength, f32 yLength, const GameObjectTag& tag)
{
	f32 halfWidth = abs(xLength * 0.5f);
	f32 halfHeight = abs(yLength * 0.5f);
	const f32 padding = 5.f;
	const f32 obs_padding = 10.f;
	const f32 grav_padding = 10.f;
	switch (tag) {
	case GameObjectTag::Obstacle:
		p_minPoint = { centrePoint.x - halfWidth + obs_padding,  centrePoint.y - halfHeight + obs_padding };
		p_maxPoint = { centrePoint.x + halfWidth - obs_padding,  centrePoint.y + halfHeight - obs_padding };
		break;
	case GameObjectTag::Player:
		p_minPoint = { centrePoint.x - halfWidth + padding,  centrePoint.y - halfHeight + padding };
		p_maxPoint = { centrePoint.x + halfWidth - padding,  centrePoint.y + halfHeight - padding };
		break;
	case GameObjectTag::GravUp:
	case GameObjectTag::GravDown:
	case GameObjectTag::GravLeft:
	case GameObjectTag::Gravright:
	case GameObjectTag::Win:
		p_minPoint = { centrePoint.x - halfWidth + grav_padding,  centrePoint.y - halfHeight + grav_padding };
		p_maxPoint = { centrePoint.x + halfWidth - grav_padding,  centrePoint.y + halfHeight - grav_padding };
		break;
	case GameObjectTag::Static:
		p_minPoint = { centrePoint.x - halfWidth + padding,  centrePoint.y - halfHeight + padding };
		p_maxPoint = { centrePoint.x + halfWidth - padding,  centrePoint.y + halfHeight - padding };
		break;
	}

}

//collider logic
bool CollisionDetected(const Collider& collider1, const Collider& collider2) {
	return !(collider1.p_maxPoint.x < collider2.p_minPoint.x ||
			 collider1.p_minPoint.x > collider2.p_maxPoint.x ||
			 collider1.p_maxPoint.y < collider2.p_minPoint.y ||
			 collider1.p_minPoint.y > collider2.p_maxPoint.y);
}



