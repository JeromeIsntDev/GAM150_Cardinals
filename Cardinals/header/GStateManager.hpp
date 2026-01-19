#pragma once
#ifndef _Game_State_Manager_
#define _Game_State_Manager_

#include "../header/GameState.hpp"

class GameStateManager {
private:
	GameState* pt_CurrentGameState;
	GameState* pt_PreviousGameState;
	GameState* pt_NextGameState;

public:
	GameStateManager(GameState* state);
	void SetCurrentGameState(GameState* state);
	GameState* GetCurrentGameState();

	void LoadCurrentState();
	void InitCurrentState();
	void UpdateCurrentState();
	void DrawCurrentState();
	void FreeCurrentState();
	void UnloadCurrentState();
};

#endif // !_Game_State_Manager_
