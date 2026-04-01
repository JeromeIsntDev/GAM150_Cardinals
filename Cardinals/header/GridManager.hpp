/**
 * @file		GridManager.hpp
 * @brief		Handles all functions related grids
 * @details		Creates, updates, draws and saves the grid with the assistance of GameSaver.cpp
 * @author		Jerome Lim
 * @date		23.02.2026
 */

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

 /**
  * @struct GridCell
  * @brief A single cell that is found in the grid; to be spawned dynamically
  */
struct GridCell {
	u32 column, row;
	AEVec2 position;
	GameObject* obj;
	GameObjectTag tag = GameObjectTag::GameObjectTagNumber;

	GridCell();
	~GridCell();

	friend std::ostream& operator<<(std::ostream& os, const GridCell& cell);
};

/**
 * @struct Grid
 * @brief  The Grid that is used for level creation 
 */
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
	Texture specialTexture{};
	GridCell* cherryCell{ nullptr };
	GridCell** cells = nullptr;
	std::vector<GameObject*> objs{};

	SpriteSheet spriteSheet;
/**
 * @brief		Intializes the Grid by spawning the gridcells and sets their corresponding screen position
 */
	void Initialize();
/**
 * @brief		Loads the texture used for the tiles 
 */
	void Load(Texture* sheet = nullptr, int sheetW = 900, int sheetH = 900, int tileW = 300, int tileH = 300);
/**
 * @brief		Clears and unloads all dynamically allocated memory related to the grid and textures
 */
	void Clear();
/**
 * @brief		Draws the Grid and all relevant objects onto the screen
 */
	void Draw() const;
/**
 * @brief		Spawn a Single Tile with the given information
 * @param tag	The GameObjectTag of the Cell
 * @param slice	The TileSheet slice of the Cell it needs it, otherwise should be 0
 * @param x		The X-axis position of the Cell in Grid Position
 * @param y		The Y-axis position of the Cell in Grid Position
 * @param r		The rotation of the Cell
 */
	void SpawnNewCell(int const& tag, int const& slice, f32 x, f32 y, f32 r);
/**
 * @brief	Deletes the Cell Object based on the given position
 */
	void DeleteCellObject(f32, f32);
/**
 * @brief	Changes the given Cell Object to a different object
 */
	void ChangeCellObject(GridCell* cell) const;
/**
 * @brief	Rotate the Cell Object based on the given position, set to the rotation
 */
	void RotateCellObject(f32, f32, s32);
/**
 * @brief	Snaps a float value to the closest Cell Center
 */
	void SnapToCell(f32*);
/**
 * @brief	Converts a given x & y in Grid Coordinates to World Coordinates
 */
	void GridToWorld(f32*, f32*);
/**
 * @brief	Converts a given x & y in World Coordinates to Grid Coordinates
 */
	void WorldToGrid(f32*, f32*);
 /**
* @brief	Find the wanted Cell based on the given position in Grid Coordinates
*/
	GridCell* GetCell(f32 x, f32 y);
};

#endif // !_GRID_MANAGER
