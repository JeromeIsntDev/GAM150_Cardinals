#pragma once
#ifndef _Game_State_Manager_
#define _Game_State_Manager_

#include <AEEngine.h>

//GameState includes
#include "../GameStates/MainMenu.hpp"
#include "../GameStates/Credits.hpp"
#include "../GameStates/LevelMenu.hpp"
#include "../GameStates/Level1.hpp"
#include "../GameStates/Level2.hpp"
#include "../GameStates/Level3.hpp"
#include "../GameStates/Level4.hpp"

typedef void(*FuncPtr)(void);

enum GameStateEnum {
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

extern GameStateEnum pt_CurrentGameState;

void UpdateGameManagerState(GameStateEnum state);
GameStateEnum GetCurrentGameState();

extern FuncPtr Load_State;
extern FuncPtr Init_State;
extern FuncPtr Update_State;
extern FuncPtr Draw_State;
extern FuncPtr Free_State;
extern FuncPtr Unload_State;
extern bool changingState;

#endif // !_Game_State_Manager_
