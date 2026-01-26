#pragma once
#ifndef _Game_State_Manager_
#define _Game_State_Manager_

#include <AEEngine.h>
#include "../header/GameState.hpp"

//GameState includes
#include "../GameStates/MainMenu.hpp"
#include "../GameStates/Credits.hpp"
#include "../GameStates/LevelMenu.hpp"
#include "../GameStates/Level1.hpp"
#include "../GameStates/Level2.hpp"
#include "../GameStates/Level3.hpp"

typedef void(*FuncPtr)(void);

enum GameStates {
	MainMenu = 0,
	Credits,
	LevelMenu,
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
		case Credits:
			return new CreditsState;
		case LevelMenu:
			return new LevelMenuState;
		case Level1:
			return new Level1State;
		case Level2:
			return new Level2State;
		case Level3:
			return new Level3State;
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
