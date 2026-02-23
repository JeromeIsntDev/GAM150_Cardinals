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
#include "GameObject.hpp"
#include "GStateManager.hpp"
#include "GameSaver.hpp"
#include "MeshGenerator.hpp"

namespace {
	bool canEdit = false;
	bool useSaveFile = false;
	GameObject lineObject;
	AEGfxVertexList* lineMesh;
}

void InitializeEditor();
void ClearEditorMemory();
void DragObject(std::vector<GameObject*> objs);
void DrawDebugObjects(std::vector<GameObject*> objs);
void DrawTiles();
void SaveGameObjectToFilepath(std::string filepathSave, std::vector<GameObject*> objs);
void ToggleEditorMode();
void ToggleSaveFileMode();
void RestartLevel(void (*restart)(GameStateEnum), GameStateEnum state);
bool IsEditorActive();
bool IsUsingSaveFile();

#endif // !_GAME_EDITOR_HEADER_
