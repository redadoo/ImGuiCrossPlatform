
#include "ImGuiCrossPlatform.hpp"

void ImGuiCrossPlatfrom::Run(std::unique_ptr<Application>& app)
{
	#ifdef __linux__
		ImGuiCrossPlatformLinux::Run(app);
	#elif _WIN32
		ImGuiCrossPlatformWin32::Run(app);
	#endif 
}
