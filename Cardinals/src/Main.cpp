/*!***********************************************************************
\file	Main.cpp
\author	Nil
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	Entry Point for project

\par	Changes: Added temp mainPlayer values - Sadric 14/1/26
**************************************************************************/
#include <crtdbg.h> // To check for memory leaks
#include <AEEngine.h>
#include <iostream>
#include "../header/GStateManager.hpp"
#include "../header/PlayerController.hpp"
#include "../header/UIManager.hpp"
#include "../header/GameObject.hpp"
#include "../header/GameManager.hpp"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("CARDINAL");

	GameStateManager manager;
	manager.SetCurrentGameState(manager.GetGameState(GameStates::MainMenu));


	while (gGameRunning)
	{
		AESysFrameStart();

		if (AEInputCheckReleased(AEVK_0)) {
			manager.SetCurrentGameState(manager.GetGameState(GameStates::MainMenu));
		}
		if (AEInputCheckReleased(AEVK_1)) {
			manager.SetCurrentGameState(manager.GetGameState(GameStates::Level1));
		}
		if (AEInputCheckReleased(AEVK_2)) {
			manager.SetCurrentGameState(manager.GetGameState(GameStates::Level2));
		}
		if (AEInputCheckReleased(AEVK_3)) {
			manager.SetCurrentGameState(manager.GetGameState(GameStates::Credits));
		}
		manager.UpdateCurrentState();
		manager.DrawCurrentState();

		AESysFrameEnd();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// free the system
	AESysExit();

}
