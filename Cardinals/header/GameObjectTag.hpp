/**
 * @file GameObjectTag.hpp
 * @brief Holds the enum class for GameObjectTags
 * @author Sadric Koh s.koh
 * @date 01.04.2026
 */
#pragma once
#ifndef TAG_HEADER_HPP
#define TAG_HEADER_HPP
enum class GameObjectTag : int {
	Static = 0,
	Dynamic = 1,
	Platform = 2,
	Player = 3,
	Win = 4,
	GravUp = 5,
	GravDown = 6,
	GravLeft = 7,
	Gravright = 8,
	Obstacle = 9,
	Special = 10,

	GameObjectTagNumber
};

#endif // !1
