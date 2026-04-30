#pragma once

#include <variant>
#include <vector>
#include <string>
#include <functional>

#if defined(USE_RAYLIB)
	#include "../graphic_backend/ImGuiCrossPlatformRaylib.h"
#elif defined(USE_GLFW)
	#include "ImGuiCrossPlatformGLFW.h"
#elif defined(__linux__)
	#include "ImGuiCrossPlatformLinux.h"
#elif defined(_WIN32)
	#include "ImGuiCrossPlatformWin32.h"
#endif

#include "../graphic_backend/BackendFlags.h"

namespace UI
{
	struct Panel
	{
		std::string name;
		std::function<void()> drawFn;
		ImGuiWindowFlags flags = 0;
	};

	using BackendVariant = std::variant<
	#if defined(USE_RAYLIB)
		ImGuiCrossPlatformRaylib
	#elif defined(USE_GLFW)
		ImGuiCrossPlatformGLFW
	#elif defined(__linux__)
		ImGuiCrossPlatformLinux
	#elif defined(_WIN32)
		ImGuiCrossPlatformWin32
	#endif
	>;

	class UIManager
	{
	public:
		int  Initialize(int width, int height, const char* title, int fps);

		void AddAttachedPanel(std::initializer_list<Panel> panels, std::function<void(ImGuiID)> layoutFn);

		void SetMinWindowSize(int width, int height);
		void SetMaxWindowSize(int width, int height);
		void MaximizeWindow();
		void MinimizeWindow();

        void SetBackendFlags(BackendFlags flags);
		void StartDrawScene();
		void EndDrawScene();
		bool ShouldQuit();
		void RequestQuit();

	private:
		BackendVariant backend;
		std::vector<Panel> m_panels;
		std::function<void(ImGuiID)> m_layoutFn;
		bool m_layoutInitialized = false;

		void DrawDockspace();

		template<typename Fn>
		auto Visit(Fn&& fn) { return std::visit(std::forward<Fn>(fn), backend); }
	};
}