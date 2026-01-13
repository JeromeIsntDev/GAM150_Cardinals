#pragma once
#ifndef _Game_State_Manager_
#define _Game_State_Manager_

#include "../header/GameState.hpp"
class GameStateManager {
private:
	GameState* pt_CurrentGameState;

public:
	GameStateManager(GameState* state);
	void SetCurrentGameState(GameState* state);
	GameState* GetCurrentGameState();

	void EnterCurrState();
	void CurrStateUpdate();
	void ExitCurrState();
};

#endif // !_Game_State_Manager_
