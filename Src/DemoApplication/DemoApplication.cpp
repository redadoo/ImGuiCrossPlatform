#include "DemoApplication.hpp"

DemoApplication::DemoApplication()
{
    image = NULL;
	this->x = true;
}

DemoApplication::~DemoApplication()
{
}

void DemoApplication::Main()
{
    ImGui::SetNextWindowSize({ 1280, 800 }, ImGuiCond_Once);
	ImGui::Begin("Demo Application", &x, ImGuiWindowFlags_NoCollapse);
	std::cout << "state : " << x << "\n";
	ImGui::End();
}

bool DemoApplication::IsOpen() const
{
	return this->x;
}
