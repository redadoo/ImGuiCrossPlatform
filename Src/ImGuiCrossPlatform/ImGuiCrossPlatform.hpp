// ImGuiCrossPlatform.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#ifdef __linux__
	#include "../ImGuiCrossPlatformImpl/ImGuiCrossPlatformLinux.hpp"
#elif _WIN32
	#include "../ImGuiCrossPlatformImpl/ImGuiCrossPlatformWin32.hpp"
#endif 

class ImGuiCrossPlatfrom
{
	public:
		static void Run(Application *app);
};

