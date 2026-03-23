#pragma once
#ifndef _GRID_MANAGER
#define _GRID_MANAGER

#include <vector>
#include <algorithm>
#include "AEEngine.h"
#include "MeshGenerator.hpp"
#include "GameObject.hpp"
#include "InputHandler.hpp"
#include "SpritesheetHandler.hpp"

struct GridCell {
	u32 column, row;
	AEVec2 position;
	GameObject* obj;
	GameObjectTag tag = GameObjectTag::GameObjectTagNumber;

	GridCell();
	~GridCell();

	friend std::ostream& operator<<(std::ostream& os, const GridCell& cell);
};

struct Grid {
	const u32 rows{ 18 };
	const u32 columns{ 32 };

	const f32 tileScale{ 50.f };
	AEGfxVertexList* tileMesh{ nullptr };
	AEGfxVertexList* standardMesh{ nullptr };
	Texture tileTexture{};
	Texture obstacleTexture{};
	Texture gravTexture{};
	Texture winTexture{};

	GridCell** cells = nullptr;
	std::vector<GameObject*> objs{};

	SpriteSheet spriteSheet;

	void Initialize();
	void Load(Texture* sheet = nullptr, int sheetW = 900, int sheetH = 900, int tileW = 300, int tileH = 300);
	void Clear();
	void Draw() const;
	void SpawnNewTile(int const&, int const&, f32, f32, f32);
	void DeleteTile(f32, f32);
	void ChangeTile(GridCell* cell) const;
	void SnapToCell(f32*);
	void GridToWorld(f32*, f32*);
	void WorldToGrid(f32*, f32*);

	GridCell* GetCell(f32 x, f32 y);
};

#endif // !_GRID_MANAGER
