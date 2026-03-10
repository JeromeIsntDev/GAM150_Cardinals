/*!***********************************************************************
\file	GameEditor.hpp
\author	Jerome Lim
\date	4/02/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	Game Editor that assists in Level Editing and other debug features.

\par	Changes: None
**************************************************************************/
#pragma once
#ifndef _GAME_EDITOR_HEADER_
#define _GAME_EDITOR_HEADER_

#include <vector>
#include <AEEngine.h>
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

void InitializeEditor();
void ClearEditorMemory();

void DragObject(std::vector<GameObject*> objs);
void DrawDebugObjects(std::vector<GameObject*> objs);
void DrawTileOutline();

void ToggleEditorMode();
void ToggleSaveFileMode();
void RestartLevel(void (*restart)(GameStateEnum), GameStateEnum state);
void SaveGameObjectToFilepath(std::string filepathSave, std::vector<GameObject*> objs);

bool IsEditorActive();
bool IsUsingSaveFile();

int& const EditorTileTag();
int& const EditorTileSlice();
void ChangeTileTag();
void ChangeTileSlice();

#endif // !_GAME_EDITOR_HEADER_
