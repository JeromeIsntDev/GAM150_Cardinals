/**
 * @file		GameEditor.hpp
 * @brief		The GameEditor to edit levels
 * @author		Jerome Lim
 * @date		03.02.2026
 */
#pragma once
#ifndef _GAME_EDITOR_HEADER_
#define _GAME_EDITOR_HEADER_

#include <vector>
#include <AEEngine.h>
#include <string>
#include "GameObject.hpp"
#include "GStateManager.hpp"
#include "GameSaver.hpp"
#include "MeshGenerator.hpp"
#include "GridManager.hpp"
#include "TextureManager.hpp"

namespace {
	bool canEdit = false;
	bool useSaveFile = false;
	GameObject lineObject;
	AEGfxVertexList* lineMesh;
	int editor_tileTag{0};
	int editor_tileSlice{ 0 };

	const f32 tileWidth{ 50.f };
	const AEVec2 tileDrawCenter{ -800.f, 450.f };
	const AEVec2 horizontalScale{ 1600.f , 1.f };
	const AEVec2 verticalScale{ 1.f , 900.f };
}

/**
 * @brief	Handles all Editor Functions; To be called in Game Level Update
 */
void EditorUpdate(Grid*, GameStateEnum, std::string const&);

/**
 * @brief	Draws the Editor Mode; Shows the Current Mode & the Grid Outline
 */
void EditorDraw(Text&, Grid&, Player&);

/**
 * @brief	Gets The Mouse Position for the Editor To Convert for Grid Coordinates
 */
void EditorGetMouse(f32&, f32&);

/**
 * @brief	Initializes Editor Variables and Textures
 */
void InitializeEditor();

/**
 * @brief	Clear and Free any memory allocated that was Initialized
 */
void ClearEditorMemory();

/**
 * @brief	Drag the object that is under the mouse during mouse one down; This Function is No Longer Used
 */
void DragObject(std::vector<GameObject*> objs);

/**
 * @brief	Draw all GameObjects Debug
 */
void DrawDebugObjects(std::vector<GameObject*> objs);

/**
 * @brief	Draws the Editor Mode; Uses GameObject DebugDraw
 */
void DrawTileOutline();

/**
 * @brief	Toggles the Editor Mode to be Drawn or not Drawn
 */
void ToggleEditorMode();

/**
 * @brief	Save the given Vector of GameObjects to the given FilePath
 */
void SaveGameObjectToFilepath(std::string filepathSave, std::vector<GameObject*> objs);

/**
 * @brief	Checks if the Editor is currently active
 */
bool IsEditorActive();

/**
 * @brief	Returns a reference to current Tile Tag; The Object Type that is be spawned
 */
int& EditorTileTag();

/**
 * @brief	Reference to the Slice of the Current Tile; Does not work with GameObject Types that have no Slices
 */
int& EditorTileSlice();


/**
 * @brief	Change the Current Tile Tag; The Object Type that is be spawned
 */
void ChangeTileTag();


/**
 * @brief	Function to handle changing the tile slice
 */
void ChangeTileSlice();

#endif // !_GAME_EDITOR_HEADER_
