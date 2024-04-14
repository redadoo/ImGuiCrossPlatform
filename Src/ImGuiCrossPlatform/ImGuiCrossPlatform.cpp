
#include "ImGuiCrossPlatform.hpp"

void ImGuiCrossPlatfrom::Run()
{
	#ifdef __linux__
		ImGuiCrossPlatformLinux::Run();
	#elif _WIN32
		ImGuiCrossPlatformWin32::Run();
	#endif 
}
