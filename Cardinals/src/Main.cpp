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
#include <vector>
#include <string>

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

static std::vector<std::string> g_logs;
//FORWARD DECLARATION;
void ControlPanelFunction();
void AddLog(const std::string& msg);

static char buffer[128] = "";
MemoryManager memoryManager;

namespace counter {
	int specialCount = 0;
}

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
	AESysInit(hInstance, nCmdShow, 1600, 900, 0, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("CARDINAL");
	AESysSetWindowIcon("../Assets/Textures/Lollipop.ico", 100, 100);
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

	//ImGuiStyle& style = ImGui::GetStyle();

	bool panelOpen = false;
	//bool focusKB = false;

	InitializeEditor();

	while (isGameRunning)
	{
		AESysFrameStart();

		//if (AEInputCheckReleased(AEVK_0)) {
		//	UpdateGameManagerState(GameStateEnum::MainMenu);
		//}
		//if (AEInputCheckReleased(AEVK_1)) {
		//	UpdateGameManagerState(GameStateEnum::Level1);
		//}
		//if (AEInputCheckReleased(AEVK_2)) {
		//	UpdateGameManagerState(GameStateEnum::Level2);
		//}
		//if (AEInputCheckReleased(AEVK_3)) {
		//	UpdateGameManagerState(GameStateEnum::Level3);
		//}
		//if (AEInputCheckReleased(AEVK_4)) {
		//	UpdateGameManagerState(GameStateEnum::Level4);
		//}
		//if (AEInputCheckReleased(AEVK_8)) {
		//	UpdateGameManagerState(GameStateEnum::LevelMenu);
		//}
		//if (AEInputCheckReleased(AEVK_9)) {
		//	UpdateGameManagerState(GameStateEnum::Credits);
		//}

		if (AEInputCheckTriggered(AEVK_F1)) {
			panelOpen = !panelOpen;
		}

		Update_State();
		Draw_State();
		//must be place here or smth
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//if log more than 200 items
		if (g_logs.size() > 200) {
			g_logs.erase(g_logs.begin());
		}
		//command panel logic goes here
		if (panelOpen) {
			ControlPanelFunction();
		}
		//render logic for IMGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		AESysFrameEnd();
		if (AEInputCheckTriggered(AEVK_DELETE) || 0 == AESysDoesWindowExist())
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
	TextureManager::GetInstance().UnloadAll();
	TextureManager::GetInstance().UnloadPersistent();
	AudioManager::Instance().Free();
	ClearEditorMemory();

	// free the system
	AESysExit();

}

void AddLog(const std::string& msg) {
	g_logs.push_back(msg);
}

void ControlPanelFunction() {
	ImGui::SetNextWindowSize(ImVec2(400.f, 400.f), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(850.f, 50.f));
	ImGui::Begin("Control Panel");
	if (ImGui::Button("Main Menu")) {
		AddLog("Loading Mainmenu");
		UpdateGameManagerState(GameStateEnum::MainMenu);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load L1")) {
		AddLog("Loading Level 1");
		UpdateGameManagerState(GameStateEnum::Level1);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load L2")) {
		AddLog("Loading Level 2");
		UpdateGameManagerState(GameStateEnum::Level2);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load L3")) {
		AddLog("Loading Level 3");
		UpdateGameManagerState(GameStateEnum::Level3);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load L4")) {
		AddLog("Loading Level 4");
		UpdateGameManagerState(GameStateEnum::Level4);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load L5")) {
		AddLog("Loading Level 5");
		UpdateGameManagerState(GameStateEnum::Level5);
	}
	if (ImGui::Button("Load Special")) {
		AddLog("Loading Level Special");
		UpdateGameManagerState(GameStateEnum::Special);
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear Log")) {
		g_logs.clear();
	}

	ImGui::Separator();

	ImGui::BeginChild("LogRegion", ImVec2(0, 120), true);

	for (const auto& log : g_logs) {
		ImGui::TextUnformatted(log.c_str());
	}

	// auto-scroll if already at bottom
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();

	ImGui::Separator();
	counter::specialCount = static_cast<int>(AEClamp(static_cast<f32>(counter::specialCount), 0, 4));
	if (ImGui::Button("Display Special Count")) {
		std::string countText{ "Count: " };
		countText += std::to_string(counter::specialCount);
		AddLog(countText);
	}
	ImGui::SameLine();
	if (ImGui::Button("Increase Special Count")) {
		std::string countText{ "Count Added: " };
		if (counter::specialCount < 4) {
			counter::specialCount++;
		}
		countText += std::to_string(counter::specialCount);
		AddLog(countText);
	}
	ImGui::SameLine();
	if (counter::specialCount > 0) {
		if (ImGui::Button("Reset Special Count")) {
			std::string countText{ "Count Resetted: " };
			counter::specialCount = 0;
			countText += std::to_string(counter::specialCount);
			AddLog(countText);
		}
	}
	ImGui::End();
}
