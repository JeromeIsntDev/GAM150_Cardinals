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
#include "../imGUI/imgui_impl_glfw.h"
#include "../imGUI/imgui_impl_opengl3.h"
#include "../imGUI/imgui_impl_opengl3_loader.h"

#include <Windows.h>
#include <GLFW/glfw3.h>
#include "../header/ControlPanel.hpp"

void ControlPanel_Load() {
	GLFWwindow* window = glfwCreateWindow(1600, 900, "Test", nullptr, nullptr);
	glfwMakeContextCurrent(window);
}

void ControlPanel_Initialize(GLFWwindow* window) {
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ControlPanel_Update() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Test");
	ImGui::Text("Test text");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ControlPanel_Exit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

