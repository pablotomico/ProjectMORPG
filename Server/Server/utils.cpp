#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <Windows.h>
#include <io.h>
#include <iostream>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>

#include "utils.h"

#pragma comment(lib, "ws2_32.lib")


void openConsoleWindow()
{
	// Allocate a console for this app.
	AllocConsole();

	// Make the screen buffer big enough to let us scroll text.
	const WORD MAX_CONSOLE_LINES = 500;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// Redirect the C IO streams (stdout etc.) to the console.
	FILE *old;
	freopen_s(&old, "CONIN$", "r", stdin);
	freopen_s(&old, "CONOUT$", "w", stdout);
	freopen_s(&old, "CONOUT$", "w", stderr);

	// Redirect the C++ IO streams (cout etc.) to the console.
	std::ios::sync_with_stdio();
}