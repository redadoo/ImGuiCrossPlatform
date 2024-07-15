#pragma once

#ifndef IMGUICROSSPLATFORMLINUX_HPP
#define IMGUICROSSPLATFORMLINUX_HPP

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include <stdio.h>
#include <SDL.h>
#if defined(imgui_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <iostream>
#include "Application.hpp"


class ImGuiCrossPlatformLinux
{
public:
	static void				Run(std::unique_ptr<Application>& app);

private:
	static bool				done;
	static SDL_Window*		window;
	static SDL_WindowFlags 	windowFlags;
	static ImGuiConfigFlags	ConfFlags;
	static SDL_GLContext	glContext;
	static void Render(std::unique_ptr<Application>& app);
	static void InitImGuiCrossPlatformLinux();
	static void CleanUp();
	static bool ShouldQuit();

};

#endif // ENGINE_H