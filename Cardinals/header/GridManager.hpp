#include <vector>
#include "AEEngine.h"
#include "GameObject.hpp"

#pragma once
#ifndef _GRID_MANAGER

struct GridCell {
	u32 column, row;
	AEVec2 position;
	GameObject* obj;

	GridCell();
	~GridCell();

	friend std::ostream& operator<<(std::ostream& os, const GridCell& cell);
};

struct Grid {
	const u32 rows{ 18 };
	const u32 columns{ 32 };

	GridCell** cells;

	Grid();
	~Grid();
	void Draw() const;

	GridCell* GetCell(f32 x, f32 y);
};

#endif // !_GRID_MANAGER
