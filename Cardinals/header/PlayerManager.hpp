#pragma once
#ifndef PLAYER_MANAGER
#define PLAYER_MANAGER

#include "../header/GameObject.hpp"

class Player : public GameObject {
private:
	f32 playerHealth;
	f32 moveSpeed;
};

#endif // !PLAYER_MANAGER
