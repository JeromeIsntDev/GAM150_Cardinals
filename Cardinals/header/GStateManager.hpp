#pragma once
#ifndef _Game_State_Manager_
#define _Game_State_Manager_

#include <AEEngine.h>
#include "../header/GameState.hpp"
#include "../GameStates/MainMenu.hpp"

enum GameStates {
	MainMenu = 0,
	Level1,
	Level2,
	Level3,
	Level4,
	Level5,

	NumberOfGameStates
};

class GameStateManager {
private:
	GameState* pt_CurrentGameState;

public:
	GameStateManager();
	void SetCurrentGameState(GameState* state);
	GameState* GetCurrentGameState();

	void LoadCurrentState();
	void InitCurrentState();
	void UpdateCurrentState();
	void DrawCurrentState();
	void FreeCurrentState();
	void UnloadCurrentState();

	static GameState* GetGameState(GameStates state) {
		switch (state)
		{
		case MainMenu:
			return new MainMenuState;
		case Level1:
			break;
		case Level2:
			break;
		case Level3:
			break;
		case Level4:
			break;
		case Level5:
			break;
		case NumberOfGameStates:
			break;
		default:
			break;
		}
	}
};

#endif // !_Game_State_Manager_
