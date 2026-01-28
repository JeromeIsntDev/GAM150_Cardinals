#pragma once
#ifndef CONTROL_PANEL_HEADER
#define CONTROL_PANEL_HEADER

#include <GLFW/glfw3.h>
#include <glad.h>
#include <AEEngine.h>


class ControlPanel {
protected:
	HWND window;
	int screenWidth;
	int screenHeight;

public:
	void ControlPanel_Load();
	void ControlPanel_Initialize();
	void ControlPanel_Update();
	void ControlPanel_Exit();
};

#endif // !CONTROL_PANEL_HEADER
