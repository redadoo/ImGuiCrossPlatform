#include "DemoApplication.hpp"

void CreateConsole()
{
	FILE* fDummy;
	AllocConsole();
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

void DemoApplication::Main()
{

	ImGui::Begin("Texture Test");
	
	if (image == NULL)
	{
		image = new Image("cat.png");
		LoadTextureFromFile(std::filesystem::absolute(image->filePath).string().c_str(), *image, &image->myImageWidth, &image->myImageHeight);
	}
	else
	{
		ImGui::Text("luca");
		ImGui::Text("pointer = %p", image->GetTexture());
		ImGui::Text("size = %d x %d", image->myImageWidth, image->myImageHeight);
		ImGui::Image(image->GetTexture(), ImVec2(200, 200));
	}
	ImGui::End();
}

DemoApplication::DemoApplication(void)
{
	CreateConsole();

	image = NULL;
}
