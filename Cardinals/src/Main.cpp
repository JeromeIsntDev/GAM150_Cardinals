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

//GAME INCLUDES
/*************************************************************************/
#include "GameObject.hpp"
#include "GameManager.hpp"
#include "GStateManager.hpp"
#include "PlayerController.hpp"
#include "UIManager.hpp"
#include "GameEditor.hpp"
#include "TextureManager.hpp"
/*************************************************************************/

//IMGUI
/*************************************************************************/
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_opengl3.h"
#include "../imGUI/imgui_impl_win32.h"
#include <glad.h>
//#include <GLFW/glfw3.h>
/*************************************************************************/

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC g_OrigWndProc = nullptr;

static LRESULT CALLBACK ImGuiHookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return 1;

	return CallWindowProc(g_OrigWndProc, hWnd, msg, wParam, lParam);
}


static char buffer[128] = "";
MemoryManager memoryManager;


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
	AESysSetWindowIcon("../../Assets/Textures/lollipop.ico", 100, 100);
	UpdateGameManagerState(GameStateEnum::MainMenu);

	HWND hwnd = AESysGetWindowHandle();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	g_OrigWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)ImGuiHookWndProc);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigWindowsResizeFromEdges = true;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	bool panelOpen = false;
	bool focusKB = false;

	InitializeEditor();

	while (isGameRunning)
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

		if (AEInputCheckTriggered(AEVK_F1)) {
			panelOpen = !panelOpen;
		}

		Update_State();
		Draw_State();
		//must be place here or smth
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//command panel logic goes here
		if (panelOpen) {
			//change hardcode to smth else later
			ImGui::SetNextWindowSize(ImVec2(400.f, 300.f), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(850.f, 50.f));
			ImGui::Begin("Panel");
			if (ImGui::Button("Main Menu")) {
				std::cout << "Loading Mainmenu\n";
				UpdateGameManagerState(GameStateEnum::MainMenu);
				std::cout << "Mainmenu loaded";
			}
			ImGui::SameLine();
			if (ImGui::Button("Load L1")) {
				std::cout << "loading L1\n";
				UpdateGameManagerState(GameStateEnum::Level1);
				std::cout << "loaded L1\n";
			}
			ImGui::End();
		}
		//render logic for IMGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		AESysFrameEnd();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
		{
			isGameRunning = 0;
			//system("pause");
		}
	}
	//resetting callback thing
	if (g_OrigWndProc) {
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)g_OrigWndProc);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Free_State();
	Unload_State();
	TextureManager::GetInstance().UnloadPersistent();
	AudioManager::Instance().Free();
	ClearEditorMemory();

	// free the system
	AESysExit();

}
