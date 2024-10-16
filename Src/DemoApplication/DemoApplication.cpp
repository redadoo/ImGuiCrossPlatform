#include "DemoApplication.hpp"

#pragma region DemoApplication Constructor/Deconstructor

DemoApplication::DemoApplication()
{
	image == nullptr;
	this->demoApplicationExitStatus = true;
}

DemoApplication::~DemoApplication() {}

#pragma endregion

#pragma region DemoApplication public function

void DemoApplication::Main()
{
	ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("Demo Application", &demoApplicationExitStatus, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
	
	if (image == nullptr)
		InitImage();
	ImGui::ImageButton("##image", image->GetTexture(), { 400, 400 });

	ImGui::End();
}

bool DemoApplication::IsOpen() const
{
	return this->demoApplicationExitStatus;
}

#pragma endregion

#pragma region private function

void DemoApplication::InitImage()
{
	std::string imagePath = std::filesystem::current_path().parent_path().string();
	imagePath += "/ImageFolder/cute-cat-relaxing-studio.jpg";
	
	image = std::make_unique<Image>(imagePath);
	LoadTextureFromFile(image->filePath, image);
}

#pragma endregion



