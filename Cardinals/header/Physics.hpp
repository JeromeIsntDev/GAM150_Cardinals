/**
 * @file Physics.hpp
 * @brief A header file with a struct for the collider and its relevant function
 * @details -
 * @author Sadric Koh s.koh
 * @date 01.04.2026
 */
#pragma once
#ifndef _Physics_Header_
#define _Physics_Header_

#include <AEEngine.h>
#include <GameObjectTag.hpp>
 /**
  * @struct Collider
  * @brief Simple collider struct for AABB collision detection
  */
struct Collider {
	//AABB
	AEVec2 p_minPoint;
	AEVec2 p_maxPoint;
	/**
	 * @brief Updates the collider position relevant to eh player position
	 *
	 * @param centrePoint the middle or currPos of the player
	 * @param xLength the xScale of the player
	 * @param yLength the yScale of the player
	 * @param tag the tag of the collider to update depending on what item it is
	 */
	void UpdateCollider(AEVec2 centrePoint, f32 xLength, f32 yLength,const GameObjectTag& tag);
};
/**
 * @brief checks if two colliders have collided
 *
 * @param first collider for the player
 * @param second collider for the item being checked with
 * @return true or false based on whether collided or not
 */
bool CollisionDetected(const Collider&, const Collider&);


#endif // !_Physics_Header_
