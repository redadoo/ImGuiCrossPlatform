
#include "../DemoApplication/DemoApplication.hpp"
#include <memory>

#ifdef __linux__
	int main()
	{
		std::unique_ptr<Application> app(new DemoApplication);

		ImGuiCrossPlatfrom::Run(app);
	}
#elif _WIN32
	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
	{
		std::unique_ptr<Application> app(new DemoApplication);

		ImGuiCrossPlatfrom::Run(app);
	}
#endif 

