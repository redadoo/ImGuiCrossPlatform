#pragma once
#include <variant>

#if defined(USE_RAYLIB)
	#include "ImGuiCrossPlatformRaylib.h"
#elif defined(__linux__)
	#include "LinuxBackend.h"
#elif defined(_WIN32)
	#include "ImGuiCrossPlatformWin32.h"
#endif

#include "BackendFlags.h"

namespace UI
{
	using BackendVariant = std::variant<
	#if defined(USE_RAYLIB)
		ImGuiCrossPlatformRaylib
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
        void SetBackendFlags(BackendFlags flags);
		void StartDrawScene();
		void EndDrawScene();
		bool ShouldQuit();
		void RequestQuit();

	private:
		BackendVariant backend;

		template<typename Fn>
		auto Visit(Fn&& fn) { return std::visit(std::forward<Fn>(fn), backend); }
	};
}