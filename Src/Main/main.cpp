#include "UI/UiManager.h"

PLATFORM_MAIN
{
	UI::UIManager manager;

	manager.SetBackendFlags(/*UI::BackendFlags::FloatingPanels |*/ UI::BackendFlags::InstantQuitWithEsc);
	manager.Initialize(1920, 1080, "Template ui", 120);

	manager.AddAttachedPanel
	(
		{
			{ "Main Panel", [&]() { ImGui::Text("ciao");} },
			{ "test", [&]() { ImGui::Text("test");} },
			{ "test 2 ", [&]() { ImGui::Text("test 2 ");} },
		},
		[](const ImGuiID root)
		{
			// Split root into left (80%) and right (20%)
			ImGuiID left, right;
			ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.80f, &right, &left);

			ImGui::DockBuilderDockWindow("Main Panel", left);
			ImGui::DockBuilderDockWindow("test", right);
			ImGui::DockBuilderDockWindow("test 2 ", right);
		}
	);

	manager.SetMinWindowSize(800, 600);
	manager.MaximizeWindow();

	while (!manager.ShouldQuit())
	{
		manager.StartDrawScene();
		manager.EndDrawScene();
	}

	return 0;
}