#include "DemoApplication.hpp"

void CreateConsole()
{
	#ifdef _WIN32
		FILE* fDummy;
		AllocConsole();
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	#endif
}

void DemoApplication::Main()
{
	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("Test");
	ImGui::End();
}

DemoApplication::DemoApplication( void )
{
	CreateConsole();

	image = NULL;
}
