#include "Platform.h"
#include "UiManager.h"
#include "imgui_internal.h"

PLATFORM_MAIN
{
	UI::UIManager manager;
	manager.SetBackendFlags(/*UI::BackendFlags::FloatingPanels |*/ UI::BackendFlags::InstantQuitWithEsc);
	manager.Initialize(1920, 1080, "Template ui", 120);

	manager.AddAttachedPanel(
		{
			{ "Main Panel", [&]() { ImGui::Text("ciao");} },
			{ "Log Banner", [&]() { ImGui::Text("log");} },
			{ "test", [&]() { ImGui::Text("test");} },
		},

		[](const ImGuiID root)
		{
			// Split root into left (80%) and right (20%)
			ImGuiID left, right;
			ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.20f, &right, &left);

			// Split left further into top and bottom
			ImGuiID top, bottom;
			ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.30f, &bottom, &top);

			ImGui::DockBuilderDockWindow("Main Panel", top);
			ImGui::DockBuilderDockWindow("Log Banner", bottom);
			ImGui::DockBuilderDockWindow("test",       right);
		}
	);

	while (!manager.ShouldQuit())
	{
		manager.StartDrawScene();

		static bool p_open = true;
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
		ImGui::Begin("Dear ImGui Demo", &p_open);
		ImGui::End();

		manager.EndDrawScene();

		if (!p_open) break;
	}

	return 0;
}