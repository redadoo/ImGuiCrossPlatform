#pragma once

#ifndef IMGUICROSSPLATFORMLINUX_HPP
#define IMGUICROSSPLATFORMLINUX_HPP

#include "../ImGui/imgui_impl_sdl2.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGui/imgui.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#include <iostream>
#include "../Application/Application.hpp"

class ImGuiCrossPlatformLinux
{
public:
	static void				Run(Application* app);

private:
	static bool				done;
	static SDL_Window*		window;
	static SDL_WindowFlags 	windowFlags;
	static ImGuiConfigFlags	ConfFlags;
	static SDL_GLContext	glContext;
	static void Render();
	static void InitImGuiCrossPlatformLinux();
	static void CleanUp();
	static void ShouldQuit();

};

#endif // ENGINE_H