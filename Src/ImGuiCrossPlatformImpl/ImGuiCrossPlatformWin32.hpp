﻿#pragma comment(lib, "d3d11.lib")
#pragma once

#ifndef imguiCROSSPLATFORMWIN32_HPP
#define imguiCROSSPLATFORMWIN32_HPP

#include <Winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include "d3d11.h"
#include "../../lib/imgui/backends/imgui_impl_dx11.h"
#include "../../lib/imgui/backends/imgui_impl_win32.h"
#include "../../lib/imgui/imgui.h"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <iostream>
#include "../Application/Application.hpp"

extern imgui_IMPL_API LRESULT imgui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// The imguiCrossPlatformWin32 contains the backend (dx11) and initialize the gui
/// </summary>
class imguiCrossPlatformWin32
{
private:
	static ID3D11DeviceContext* pd3dDeviceContext;
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pMainRenderTargetView;

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	static ID3D11Device* pd3dDevice;
	inline static imguiConfigFlags   ConfFlags;
	inline static HWND hwnd;
	inline static WNDCLASSEX wc;
	static HMODULE hCurrentModule;
	inline static bool bDone = false;
	inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static void Render(Application* app);
	static void Initimgui();
	static void CleanUp();
	static bool ShouldQuit();
	static void Run(Application* app);
};

#endif
