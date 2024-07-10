#pragma once

#ifndef imguiCROSSPLATFORMLINUX_HPP
#define imguiCROSSPLATFORMLINUX_HPP

#include "../../lib/imgui/backends/imgui_impl_sdl2.h"
#include "../../lib/imgui/backends/imgui_impl_opengl3.h"
#include "../../lib/imgui/imgui.h"
#include <stdio.h>
#include <SDL.h>
#if defined(imgui_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <iostream>
#include "../Application/Application.hpp"


class imguiCrossPlatformLinux
{
public:
	static void				Run(Application* app);

private:
	static bool				done;
	static SDL_Window*		window;
	static SDL_WindowFlags 	windowFlags;
	static imguiConfigFlags	ConfFlags;
	static SDL_GLContext	glContext;
	static void Render(Application* app);
	static void InitimguiCrossPlatformLinux();
	static void CleanUp();
	static bool ShouldQuit();

};

#endif // ENGINE_H