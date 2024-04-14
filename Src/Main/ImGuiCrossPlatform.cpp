// ImGuiCrossPlatform.cpp : Defines the entry point for the application.
//

#include "ImGuiCrossPlatform.hpp"

int main()
{
	std::cout << "va ?\n";
	ImGuiCrossPlatformLinux test = ImGuiCrossPlatformLinux();
	test.Run();
	return 0;
}
