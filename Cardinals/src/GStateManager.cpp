#include "../header/GStateManager.hpp"

FuncPtr Load_State{}, Init_State{}, Update_State{}, 
Draw_State{}, Free_State{}, Unload_State{};

bool changingState{ 0 };

GameStateEnum pt_CurrentGameState = NumberOfGameStates;


void UpdateGameManagerState(GameStateEnum state)
{
	//if (state == pt_CurrentGameState) return;
	if(Free_State) Free_State();
	if(Unload_State) Unload_State();
	
	//player pointer

	pt_CurrentGameState = state;
	switch (state)
	{
	case MainMenu:
		Load_State = MainMenu_Load;
		Init_State = MainMenu_Initialize;
		Update_State = MainMenu_Update;
		Draw_State = MainMenu_Draw;
		Free_State = MainMenu_Free;
		Unload_State = MainMenu_Unload;
		break;
	case Credits:
		Load_State = Credits_Load;
		Init_State = Credits_Initialize;
		Update_State = Credits_Update;
		Draw_State = Credits_Draw;
		Free_State = Credits_Free;
		Unload_State = Credits_Unload;
		break;
	case LevelMenu:
		Load_State = LevelMenu_Load;
		Init_State = LevelMenu_Initialize;
		Update_State = LevelMenu_Update;
		Draw_State = LevelMenu_Draw;
		Free_State = LevelMenu_Free;
		Unload_State = LevelMenu_Unload;
		break;
	case Level1:
		Load_State = Level1_Load;
		Init_State = Level1_Initialize;
		Update_State = Level1_Update;
		Draw_State = Level1_Draw;
		Free_State = Level1_Free;
		Unload_State = Level1_Unload;
		break;
	case Level2:
		Load_State = Level2_Load;
		Init_State = Level2_Initialize;
		Update_State = Level2_Update;
		Draw_State = Level2_Draw;
		Free_State = Level2_Free;
		Unload_State = Level2_Unload;
		break;
	case Level3:
		Load_State = Level3_Load;
		Init_State = Level3_Initialize;
		Update_State = Level3_Update;
		Draw_State = Level3_Draw;
		Free_State = Level3_Free;
		Unload_State = Level3_Unload;
		break;
	case Level4:
		Load_State = Level4_Load;
		Init_State = Level4_Initialize;
		Update_State = Level4_Update;
		Draw_State = Level4_Draw;
		Free_State = Level4_Free;
		Unload_State = Level4_Unload;
		break;
	case Level5:
		Load_State = Level5_Load;
		Init_State = Level5_Initialize;
		Update_State = Level5_Update;
		Draw_State = Level5_Draw;
		Free_State = Level5_Free;
		Unload_State = Level5_Unload;
		break;
	case Special:
		Load_State = LevelS_Load;
		Init_State = LevelS_Initialize;
		Update_State = LevelS_Update;
		Draw_State = LevelS_Draw;
		Free_State = LevelS_Free;
		Unload_State = LevelS_Unload;
		break;
	default:
		break;
	}
	Load_State();
	Init_State();
}

GameStateEnum GetCurrentGameState()
{
	return pt_CurrentGameState;
}
