
#include "../ImGuiCrossPlatform/ImGuiCrossPlatform.hpp"

#ifdef __linux__
	int main()
	{
		ImGuiCrossPlatfrom::Run();
	}
#elif _WIN32
	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
	{
		ImGuiCrossPlatfrom::Run();
	}
#endif 