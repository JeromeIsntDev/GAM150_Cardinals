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

namespace {
	bool canEdit = false;
}

void DragObject(std::vector<GameObject*> objs);
void SaveGameObjectToFilepath(std::string filepathSave, std::vector<GameObject*> objs);
void ToggleEditorMode();
void RestartLevel(void (*restart)(GameStateEnum), GameStateEnum state);
bool IsEditorActive();


#endif // !_GAME_EDITOR_HEADER_
