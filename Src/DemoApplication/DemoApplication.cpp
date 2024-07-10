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
	imgui::SetNextWindowSize({ 1280, 800 }, imguiCond_Once);
	imgui::Begin("Demo Application", &x);
	imgui::End();
}

bool DemoApplication::IsClosed()
{
	return this->x;
}

DemoApplication::DemoApplication( void )
{

#if defined _DEBUG
		CreateConsole();
#endif

	image = NULL;
	this->x = false;
}
