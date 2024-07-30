
#include "../DemoApplication/DemoApplication.hpp"
#include <memory>

#ifdef __linux__
	int main()
	{
		std::unique_ptr<Application> app(new DemoApplication);

		ImGuiCrossPlatfrom::Run(app);
	}
#elif _WIN32

	/// <summary>
	///  attach a console to the programm (for debug purpose)
	/// </summary>
	void CreateConsole()
	{
		FILE* fDummy;
		AllocConsole();
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}



	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
	{

#if defined _DEBUG
		CreateConsole();
		std::cout << "debug build\n";
#endif

		std::unique_ptr<Application> app(new DemoApplication);

		ImGuiCrossPlatfrom::Run(app);
	}
#endif 

