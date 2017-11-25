#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>


#include "resource.h"

#define MAX_LOADSTRING 100

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawWindow(HWND window);
void CloseWindow();

void HandleSocketEvent(LPARAM, WPARAM wParam);
