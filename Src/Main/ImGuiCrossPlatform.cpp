
#include "ImGuiCrossPlatform.hpp"


#ifdef __linux__
int main()
{
	ImGuiCrossPlatformLinux::Run();
}
#elif _WIN32
	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
	{
		ImGuiCrossPlatformWin32::InitImGui();
		ImGuiCrossPlatformWin32::Render();
		ImGuiCrossPlatformWin32::CleanUp();
		return 0;
	}
#endif 

