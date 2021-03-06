#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

class WindowManager {
public:
	void Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int windowWidth, int windowHeight);
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	Window window;
	Keyboard keyboard;
	Mouse mouse;
};