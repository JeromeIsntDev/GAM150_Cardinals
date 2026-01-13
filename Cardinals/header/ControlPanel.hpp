#pragma once
#ifndef CONTROL_PANEL_HEADER
#define CONTROL_PANEL_HEADER

#include <Windows.h>
#include <gl/gl.h>
#include <GLFW/glfw3.h>

class gui {
	//const window size
	int WIDTH = 500;
	int HEIGHT = 300;

	bool exit = true;
	HWND window = nullptr;
	WNDCLASSEXA windowClass = {};

	POINTS position = {};

	
};

#endif // !CONTROL_PANEL_HEADER
