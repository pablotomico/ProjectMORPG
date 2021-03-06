#include <iostream>
#include <list>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "utils.h"
#include "Window.hpp"
#include "Network.hpp"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	openConsoleWindow();
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}
	
	
	
	MSG msg;
	
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return (int) msg.wParam;
}