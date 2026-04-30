#include "graphic_backend/Platform.h"
#include "UI/UiManager.h"
#include "imgui_internal.h"

PLATFORM_MAIN
{
	UI::UIManager manager;
	manager.SetBackendFlags(/*UI::BackendFlags::FloatingPanels |*/ UI::BackendFlags::InstantQuitWithEsc);
	manager.Initialize(1920, 1080, "Template ui", 120);

	manager.AddAttachedPanel(
		{
			{ "Main Panel", [&]() { ImGui::Text("ciao");} },
			{ "test", [&]() { ImGui::Text("test");} },
		},

		[](const ImGuiID root)
		{
			// Split root into left (80%) and right (20%)
			ImGuiID left, right;
			ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.20f, &right, &left);

			ImGui::DockBuilderDockWindow("Main Panel", left);
			ImGui::DockBuilderDockWindow("test", right);
		}
	);

	manager.SetMinWindowSize(800, 600);
	manager.MaximizeWindow();

	while (!manager.ShouldQuit())
	{
		manager.StartDrawScene();

		ImGui::ShowDemoWindow();
		static bool p_open = true;
		ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
		ImGui::Begin("Dear ImGui Demo", &p_open);
		ImGui::End();

		manager.EndDrawScene();

		if (!p_open) break;
	}

	return 0;
}