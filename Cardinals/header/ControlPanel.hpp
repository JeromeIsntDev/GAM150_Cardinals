#pragma once
#ifndef CONTROL_PANEL_HEADER
#define CONTROL_PANEL_HEADER

#include <Windows.h>
#include <gl/gl.h>
#include <GLFW/glfw3.h>


class ControlPanel {
protected:
	GLFWwindow* window;
	int screenWidth;
	int screenHeight;

public:
	void ControlPanel_Load();
	void ControlPanel_Initialize(GLFWwindow* window);
	void ControlPanel_Update();
	void ControlPanel_Exit();
};

#endif // !CONTROL_PANEL_HEADER
