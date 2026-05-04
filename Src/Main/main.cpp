#include "UI/UiManager.h"
#include <chrono>
#include <thread>

PLATFORM_MAIN
{
	UI::UIManager manager;

	manager.SetBackendFlags(UI::BackendFlags::FloatingPanels | UI::BackendFlags::InstantQuitWithEsc);
	manager.Initialize(1920, 1080, "Template ui", 120);

	// manager.AddAttachedPanel
	// (
	// 	{
	// 		{ "Main Panel", [&]() { ImGui::Text("ciao");} },
	// 		{ "test", [&]() { ImGui::Text("test");} },
	// 		{ "test 2 ", [&]() { ImGui::Text("test 2 ");} },
	// 	},
	// 	[](const ImGuiID root)
	// 	{
	// 		// Split root into left (80%) and right (20%)
	// 		ImGuiID left, right;
	// 		ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.80f, &right, &left);
	//
	// 		ImGui::DockBuilderDockWindow("Main Panel", left);
	// 		ImGui::DockBuilderDockWindow("test", right);
	// 		ImGui::DockBuilderDockWindow("test 2 ", right);
	// 	}
	// );

	manager.SetMinWindowSize(800, 600);
	manager.MaximizeWindow();

	const int activeFPS = 60;
	const int idleFPS   = 5;

	while (!manager.ShouldQuit())
	{
		auto frameStart = std::chrono::high_resolution_clock::now();

		manager.StartDrawScene();
		ImGui::ShowDemoWindow();
		manager.EndDrawScene();

		ImGuiIO& io = ImGui::GetIO();

		bool isActive =
			io.WantCaptureMouse ||
			io.WantCaptureKeyboard ||
			io.MouseDelta.x != 0.0f ||
			io.MouseDelta.y != 0.0f ||
			ImGui::IsAnyItemActive() ||
			ImGui::IsAnyItemHovered();

		int targetFPS = isActive ? activeFPS : idleFPS;

		auto frameTime = std::chrono::milliseconds(1000 / targetFPS);

		auto frameEnd = std::chrono::high_resolution_clock::now();
		auto elapsed  = frameEnd - frameStart;

		if (elapsed < frameTime)
			std::this_thread::sleep_for(frameTime - elapsed);
	}

	return 0;
}