#pragma once

#ifndef IMGUICROSSPLATFORM_HPP
#define IMGUICROSSPLATFORM_HPP

#ifdef __linux__
	#include "ImGuiCrossPlatformLinux.hpp"
#elif _WIN32
	#include "ImGuiCrossPlatformWin32.hpp"
#endif

class ImGuiCrossPlatfrom
{
public:
	static void Run(std::unique_ptr<Application>& app);
};


#endif // !IMGUICROSSPLATFORM_HPP



