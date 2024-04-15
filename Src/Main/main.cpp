
#include "../ImGuiCrossPlatform/ImGuiCrossPlatform.hpp"
#include "../DemoApplication/DemoApplication.hpp"

#ifdef __linux__
	int main()
	{
		Application* app = new DemoApplication();

		ImGuiCrossPlatfrom::Run(app);

		delete app;
	}
#elif _WIN32
	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
	{
		Application *app = new DemoApplication();

		ImGuiCrossPlatfrom::Run(app);

		delete app;
	}
#endif 

