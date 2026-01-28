/*!***********************************************************************
\file	ControlPanel.cpp
\author	Sadric Koh
\date	12/01/2026
\par	Last Updated by: Sadric Koh
\par	Project: Cardinals

\brief	Controller for Control Panel Display

\par	Changes: -
**************************************************************************/

#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_opengl3.h"
#include "../imGUI/imgui_impl_win32.h"

#include <glad.h>
#include <GLFW/glfw3.h>

#include <AEEngine.h>
#include "../header/ControlPanel.hpp"

void ControlPanel::ControlPanel_Load() { 
}


void ControlPanel::ControlPanel_Initialize() {
	HWND hwnd = AESysGetWindowHandle();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigWindowsResizeFromEdges = true;
}

void ControlPanel::ControlPanel_Update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	ImGui::Text("Test text");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ControlPanel::ControlPanel_Exit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

