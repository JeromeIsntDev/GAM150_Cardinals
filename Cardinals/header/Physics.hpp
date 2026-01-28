#pragma once
#ifndef _Physics_Header_
#define _Physics_Header_

#include <AEEngine.h>

class Collider {
	//AABB
	AEVec2 p_minPoint;
	AEVec2 p_maxPoint;

public:
	void UpdateCollider(AEVec2 centrePoint, f32 xLength, f32 yLtength);

	bool CollisionDetected(const Collider&, const Collider&);

};


#endif // !_Physics_Header_
