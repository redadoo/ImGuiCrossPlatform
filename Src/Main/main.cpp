#include "Platform.h"
#include "UiManager.h"

PLATFORM_MAIN
{
	UI::UIManager manager;
	manager.SetBackendFlags(UI::BackendFlags::FloatingPanels | UI::BackendFlags::InstantQuitWithEsc);
	manager.Initialize(1920, 1080, "Template ui", 120);

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