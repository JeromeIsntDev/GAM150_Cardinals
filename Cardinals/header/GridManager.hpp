#include <vector>
#include "AEEngine.h"
#include "GameObject.hpp"

#pragma once
#ifndef _GRID_MANAGER

struct GridCell {
	AEVec2 position;
	GameObject* cellObj;
};

struct Grid {
	const u32 rows{ 18 };
	const u32 columns{ 32 };

	GridCell** grid;

	Grid();
	~Grid();
	void Draw();

	GridCell GetCell(f32 x, f32 y);
};

#endif // !_GRID_MANAGER
