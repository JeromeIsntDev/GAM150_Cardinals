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

/*
IMGUI
*/
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_opengl3.h"
#include "../imGUI/imgui_impl_win32.h"

#include <glad.h>
#include <GLFW/glfw3.h>

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

	UpdateGameManagerState(GameStateEnum::MainMenu);

	HWND hwnd = AESysGetWindowHandle();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsResizeFromEdges = true;
	io.DisplaySize = { 500.f,500.f };

	bool panelOpen = false;

	while (gGameRunning)
	{
		AESysFrameStart();

		if (AEInputCheckReleased(AEVK_0)) {
			UpdateGameManagerState(GameStateEnum::MainMenu);
		}
		if (AEInputCheckReleased(AEVK_1)) {
			UpdateGameManagerState(GameStateEnum::Level1);
		}
		if (AEInputCheckReleased(AEVK_2)) {
			UpdateGameManagerState(GameStateEnum::Level2);
		}
		if (AEInputCheckReleased(AEVK_3)) {
			UpdateGameManagerState(GameStateEnum::Credits);
		}


		Update_State();
		Draw_State();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Test");
		ImGui::Text("Test text");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		AESysFrameEnd();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Free_State();
	Unload_State();

	// free the system
	AESysExit();

}
