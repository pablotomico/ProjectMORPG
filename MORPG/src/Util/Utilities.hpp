#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define RUNNING_WINDOWS
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#ifdef RUNNING_WINDOWS
void PauseApplication() noexcept { system("pause"); }
#else
void PauseApplication() noexcept { system("read"); }
#endif


#if _DEBUG

#define LOG(x) do { std::cout << x << std::endl; } while (0)
#define LOGX(x) do { std::cout << #x << ": " << x << std::endl; } while (0)

#define DEBUG(x) do { std::cerr << x << std::endl; } while (0)
#define DEBUGX(x) do { std::cerr << #x << ": " << x << std::endl; } while (0)

#else

#define LOG(x)
#define LOGX(x)

#define DEBUG(x)
#define DEBUGX(x)

#endif


namespace Utils {
#ifdef RUNNING_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlwapi.h>
	inline std::string GetWorkingDirectory() {
		HMODULE hModule = GetModuleHandle(nullptr);
		if (hModule) {
			wchar_t path[256];
			GetModuleFileName(hModule, path, sizeof path);
			PathRemoveFileSpec(path);
			std::wstring ws(path);
			std::string p(ws.begin(), ws.end());
			p += "\\";

			return p;
		}
		return "";
	}
#elif defined RUNNING_LINUX
#include <unistd.h>
	inline std::string GetWorkingDirectory() {
		char cwd[1024];
		if (getcwd(cwd, sizeof cwd) != nullptr) {
			return std::string(cwd) + std::string("/");
		}
		return "";
	}
#endif
}