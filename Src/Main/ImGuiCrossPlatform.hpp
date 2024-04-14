// ImGuiCrossPlatform.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#ifdef __linux__
	#include "../ImGuiCrossPlatform/ImGuiCrossPlatformLinux.hpp"
#elif _WIN32
	#include "../ImGuiCrossPlatform/ImGuiCrossPlatformWin32.hpp"
#endif 

