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

class ImGuiCrossPlatformLinux
{
public:
	void				Run();
	bool				done;
	SDL_Window*			window;
	SDL_WindowFlags 	windowFlags;
	ImGuiConfigFlags   	ConfFlags;
	SDL_GLContext		glContext;

private:
	void Render();
	void InitImGuiCrossPlatformLinux();
	void CleanUp();
	void ShouldQuit();

};

#endif // ENGINE_H