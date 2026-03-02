#include <vector>
#include <algorithm>
#include "AEEngine.h"
#include "MeshGenerator.hpp"
#include "GameObject.hpp"
#include "InputHandler.hpp"

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

	AEGfxVertexList* tileMesh;
	Texture tileTexture;

	GridCell** cells;
	std::vector<GameObject*> objs;

	void Initialize();
	void Load();
	void Clear();
	void Draw() const;
	void SpawnNewTile(int const&, f32, f32);
	void DeleteTile(f32, f32);

	GridCell* GetCell(f32 x, f32 y);
	//std::vector<GameObject*> GetCellGameObject() const;
};

#endif // !_GRID_MANAGER
