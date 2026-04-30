#pragma once

#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "BackendFlags.h"

struct RaylibContext
{
	int width = 0;
	int height = 0;
	int fps = 60;
	const char* title = "";
	Color clearColor = { 114, 144, 154, 255 };
	bool done = false;
	UI::BackendFlags backendFlags = UI::BackendFlags::None;
};

struct ImGuiCrossPlatformRaylib
{
	RaylibContext ctx;

	inline void SetFlags(UI::BackendFlags flags)
	{
		ctx.backendFlags = flags;
	}

	inline bool Init(const char* windowTitle, int width, int height, int fps)
	{
		ctx.title  = windowTitle;
		ctx.width  = width;
		ctx.height = height;
		ctx.fps    = fps;

		InitWindow(width, height, windowTitle);
		SetTargetFPS(fps);
		rlImGuiSetup(true);
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		return true;
	}

	inline void BeginFrame() const
	{
		BeginDrawing();
		ClearBackground(ctx.clearColor);
		rlImGuiBegin();
	}

	static inline void EndFrame()
	{
		rlImGuiEnd();
		EndDrawing();
	}

	inline bool ShouldQuit()
	{
		if (WindowShouldClose())
			ctx.done = true;

		if (HasFlag(ctx.backendFlags, UI::BackendFlags::InstantQuitWithEsc) && IsKeyPressed(KEY_ESCAPE))
			ctx.done = true;

		return ctx.done;
	}

	static inline void Shutdown()
	{
		rlImGuiShutdown();
		CloseWindow();
	}

};