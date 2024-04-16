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
	ImGui::Begin("Texture Test1");
	
	if (image == NULL)
	{
		image = new Image("cat.png");
		LoadTextureFromFile(std::filesystem::absolute(image->filePath).string().c_str(), *image, &image->myImageWidth, &image->myImageHeight);
	}
	else
	{
		ImGui::Text("pointer = %p", image->GetTexture());
		ImGui::Text("size = %d x %d", image->myImageWidth, image->myImageHeight);
		ImGui::Image(image->GetTexture(), ImVec2(200, 200));
	}
	ImGui::End();
}

DemoApplication::DemoApplication( void )
{
	CreateConsole();

	image = NULL;
}
