#include "UI/UiManager.h"

#include <iostream>
#include <ostream>

#include "imgui_internal.h"
using namespace UI;

int UIManager::Initialize(int width, int height, const char* title, int fps)
{
	bool ok = false;
	Visit([&](auto& b) {
		ok = b.Init(title, width, height, fps);
	});

	if (!ok) return 1;
	return 0;
}

void UIManager::AddAttachedPanel(std::initializer_list<Panel> panels, std::function<void(ImGuiID)> layoutFn)
{
	bool floating = false;
	Visit([&](auto& b) { floating = HasFlag(b.ctx.backendFlags, BackendFlags::FloatingPanels); });
	if (floating) return;

	m_panels.assign(panels.begin(), panels.end());
	m_layoutFn = std::move(layoutFn);
	m_layoutInitialized = false;
}

void UIManager::SetMinWindowSize(int width, int height)
{
	Visit([&](auto& b) {
		b.SetMinWindowSize(width, height);
	});
}

void UIManager::SetMaxWindowSize(int width, int height)
{
	Visit([&](auto& b) {
		b.SetMaxWindowSize(width, height);
	});
}
void UIManager::MaximizeWindow()
{
	Visit([&](auto& b) {
		b.MaximizeWindow();
	});
}

void UIManager::MinimizeWindow()
{
	Visit([&](auto& b) {
		b.MinimizeWindow();
	});
}

void UIManager::DrawDockspace()
{
	const ImGuiViewport* vp = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(vp->WorkPos);
	ImGui::SetNextWindowSize(vp->WorkSize);
	ImGui::SetNextWindowViewport(vp->ID);

	constexpr ImGuiWindowFlags hostFlags =
		ImGuiWindowFlags_NoDecoration          |
		ImGuiWindowFlags_NoMove                |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus            |
		ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("##DockHost", nullptr, hostFlags);
	ImGui::PopStyleVar(2);

	const ImGuiID dsId = ImGui::GetID("##MainDS");

	ImGui::DockSpace(dsId, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

	if (!m_layoutInitialized && m_layoutFn)
	{
		ImGui::DockBuilderRemoveNode(dsId);
		ImGui::DockBuilderAddNode(dsId, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dsId, vp->WorkSize);
		m_layoutFn(dsId);
		ImGui::DockBuilderFinish(dsId);
		m_layoutInitialized = true;
	}

	ImGui::End();

	for (const auto& panel : m_panels)
	{
		if (ImGui::Begin(panel.name.c_str(), nullptr, panel.flags))
			panel.drawFn();
		ImGui::End();
	}
}

void UIManager::SetBackendFlags(BackendFlags flags)
{
	Visit([flags](auto& b) { b.SetFlags(flags); });
}

void UIManager::StartDrawScene()
{
	Visit([](auto& b) { b.BeginFrame(); });
	if (!m_panels.empty())
		DrawDockspace();
}

void UIManager::EndDrawScene()
{
	Visit([](auto& b) { b.EndFrame(); });
}

bool UIManager::ShouldQuit()
{
	return Visit([](auto& b) { return b.ShouldQuit(); });
}

void UIManager::RequestQuit()
{
	Visit([](auto& b) { b.Shutdown(); });
}