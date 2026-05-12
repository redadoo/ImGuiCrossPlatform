#pragma once

#include <variant>
#include <vector>
#include <string>
#include <functional>
#include <initializer_list>
#include <utility>

#if defined(USE_RAYLIB)
    #include "graphic_backend/ImGuiCrossPlatformRaylib.h"
#elif defined(USE_GLFW)
    #include "graphic_backend/ImGuiCrossPlatformGLFW.h"
#elif defined(__linux__)
    #include "graphic_backend/ImGuiCrossPlatformLinux.h"
#elif defined(_WIN32)
    #include "graphic_backend/ImGuiCrossPlatformWin32.h"
#endif

#include "graphic_backend/BackendFlags.h"
#include "imgui_internal.h"
#include "graphic_backend/Platform.h"

namespace UI
{
    enum class PanelState
    {
        Normal,
        Minimized,
        Fullscreen
    };

    struct Panel
    {
        std::string name;
        std::function<void()> drawFn;
        ImGuiWindowFlags flags = 0;
        ImVec2 size = ImVec2(0, 0);
        ImVec2 pos = ImVec2(0, 0);
        PanelState state = PanelState::Normal;

        ImVec2 normalSize = ImVec2(0, 0);
        ImVec2 normalPos  = ImVec2(0, 0);

        ImVec2 currentSize = ImVec2(0, 0);
        ImVec2 currentPos  = ImVec2(0, 0);

        bool sizeCaptured = false;
        bool panelSet = false;

    };

    using BackendVariant = std::variant<
    #if defined(USE_RAYLIB)
        ImGuiCrossPlatformRaylib
    #elif defined(USE_GLFW)
        ImGuiCrossPlatformGLFW
    #elif defined(__linux__)
        ImGuiCrossPlatformLinux
    #elif defined(_WIN32)
        ImGuiCrossPlatformWin32
    #endif
    >;

    class UIManager
    {
    public:
        template<typename Fn>
        inline decltype(auto) Visit(Fn&& fn)
        {
            return std::visit(std::forward<Fn>(fn), backend);
        }

        int Initialize(int width, int height, const char* title, int fps)
        {
            bool ok = false;
            Visit([&](auto& b) { ok = b.Init(title, width, height, fps); });
            m_initialized = ok;
            return ok ? 0 : 1;
        }

        void AddPanel(std::string name, std::function<void()> drawFn, ImGuiWindowFlags flags = 0, const ImVec2 size = ImVec2(0, 0), const ImVec2 pos = ImVec2(0, 0))
        {
            if (isFloating)
                flags |= ImGuiWindowFlags_MenuBar;
            m_panels.push_back({ std::move(name), std::move(drawFn), flags, size, pos });
        }

        void SetLayout(std::function<void(ImGuiID)> layoutFn)
        {
            bool floating = false;
            Visit([&](auto& b) {
                floating = HasFlag(b.ctx.backendFlags, BackendFlags::FloatingPanels);
            });

            UI_ASSERT(!floating, "SetLayout cannot be used with FloatingPanels flag set.");

            m_layoutFn = std::move(layoutFn);
            m_layoutInitialized = false;
        }

        void AddAttachedPanel(std::initializer_list<Panel> panels,
                              std::function<void(ImGuiID)> layoutFn)
        {
            bool floating = false;
            Visit([&](auto& b) {
                floating = HasFlag(b.ctx.backendFlags, BackendFlags::FloatingPanels);
            });

            UI_ASSERT(!floating, "AddAttachedPanel cannot be used with FloatingPanels flag set.");

            m_panels.assign(panels.begin(), panels.end());
            m_layoutFn = std::move(layoutFn);
            m_layoutInitialized = false;
        }

        bool IsMaximized()
        {
            return Visit([](auto& b) {
                return b.IsMaximized();
            });
        }

        void RestoreWindow()
        {
            Visit([](auto& b) {
                b.RestoreWindow();
            });
        }

        void SetMinWindowSize(int width, int height)
        {
            Visit([&](auto& b) {
                b.SetMinWindowSize(width, height);
            });
        }

        void SetMaxWindowSize(int width, int height)
        {
            Visit([&](auto& b) {
                b.SetMaxWindowSize(width, height);
            });
        }

        void MaximizeWindow()
        {
            Visit([](auto& b) {
                if (!HasFlag(b.ctx.backendFlags, BackendFlags::FloatingPanels))
                    b.MaximizeWindow();
            });
        }

        void MinimizeWindow()
        {
            Visit([](auto& b) {
                if (!HasFlag(b.ctx.backendFlags, BackendFlags::FloatingPanels))
                    b.MinimizeWindow();
            });
        }

        void SetBackendFlags(BackendFlags flags)
        {
            UI_ASSERT(!m_initialized, "SetBackendFlags must be called before Initialize.");
            Visit([flags](auto& b) { b.SetFlags(flags); });
            if ((static_cast<unsigned int>(flags) & static_cast<unsigned int>(BackendFlags::FloatingPanels)) != 0)
                isFloating = true;
        }

        ImVec2 GetDesktopSize()
        {
            return Visit([](auto& b) {
                return b.GetDesktopSize();
            });
        }

        void StartDrawScene()
        {
            Visit([](auto& b) {
                b.BeginFrame();
            });

            if (!m_panels.empty())
                DrawDockspace();
        }

        void EndDrawScene()
        {
            Visit([](auto& b) {
                b.EndFrame();
            });
        }

        bool ShouldQuit()
        {
            return Visit([](auto& b) {
                return b.ShouldQuit();
            });
        }

        void RequestQuit()
        {
            Visit([](auto& b) {
                b.Shutdown();
            });
        }

        void MaximizePanel(const std::string& name)
        {
            for (auto& p : m_panels)
            {
                if (p.name == name)
                {
                    if (p.state != PanelState::Fullscreen)
                    {
                        p.normalSize = p.size;
                        p.normalPos  = p.pos;
                        p.sizeCaptured = true;
                    }

                    p.state = PanelState::Fullscreen;
                }
            }
        }

        void MinimizePanel(const std::string& name)
        {
            for (auto& p : m_panels)
                if (p.name == name)
                    p.state = PanelState::Minimized;
        }

        void RestorePanel(const std::string& name)
        {
            for (auto& p : m_panels)
            {
                if (p.name == name)
                {
                    p.state = PanelState::Normal;

                    if (p.sizeCaptured)
                    {
                        p.size = p.normalSize;
                        p.pos  = p.normalPos;
                    }
                }
            }
        }

        bool IsPanelFullscreen(const std::string& name) const {
            for (auto& p : m_panels)
                if (p.name == name)
                    return p.state == PanelState::Fullscreen;
            return false;
        }

    private:
        BackendVariant backend;
        std::vector<Panel> m_panels;
        std::function<void(ImGuiID)> m_layoutFn;
        bool m_layoutInitialized = false;
        bool m_initialized = false;
        bool isFloating = false;

        inline void DrawPanelChrome(Panel& panel) const {
            if (!isFloating)
                return;

            if (!(panel.flags & ImGuiWindowFlags_MenuBar))
                return;

            if (!ImGui::BeginMenuBar())
                return;

            ImGui::TextUnformatted(panel.name.c_str());

            constexpr float btnSize = 22.0f;
            constexpr float spacing = 4.0f;
            constexpr ImVec2 size(btnSize, btnSize);

            constexpr float totalWidth = (btnSize * 2.0f) + spacing;

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalWidth);

            const bool fs = (panel.state == PanelState::Fullscreen);

            if (ImGui::Button("[]", size))
            {
                panel.state = fs ? PanelState::Normal : PanelState::Fullscreen;
            }

            ImGui::SameLine(0, spacing);

            if (ImGui::Button("X", size))
            {
                panel.state = PanelState::Minimized;
            }

            ImGui::EndMenuBar();
        }

        inline void DrawDockspace()
        {
            const ImGuiViewport* vp = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(vp->WorkPos);
            ImGui::SetNextWindowSize(vp->WorkSize);
            ImGui::SetNextWindowViewport(vp->ID);

            constexpr ImGuiWindowFlags hostFlags =
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoBackground;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            ImGui::Begin("##DockHost", nullptr, hostFlags);
            ImGui::PopStyleVar(2);

            const ImGuiID dsId = ImGui::GetID("##MainDS");

            ImGui::DockSpace(dsId, ImVec2(0, 0),
                             ImGuiDockNodeFlags_PassthruCentralNode);

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

            for (auto& panel : m_panels)
            {
                if (panel.state == PanelState::Minimized)
                    continue;

                ImGuiWindowFlags flags = panel.flags;

                if (panel.state == PanelState::Fullscreen)
                {
                    panel.panelSet = false;
                    ImGuiViewport* vp = ImGui::GetMainViewport();

                    if (const ImGuiWindow* win = ImGui::FindWindowByName(panel.name.c_str()))
                        vp = win->Viewport;

                    panel.currentPos  = vp->WorkPos;
                    panel.currentSize = vp->WorkSize;

                    ImGui::SetNextWindowPos(vp->WorkPos, ImGuiCond_Always);
                    ImGui::SetNextWindowSize(vp->WorkSize, ImGuiCond_Always);
                    ImGui::SetNextWindowViewport(vp->ID);

                    flags |= ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse;
                }
                else
                {
                    if (panel.pos.x != 0 || panel.pos.y != 0 && !panel.panelSet)
                    {
                        ImGui::SetNextWindowPos(panel.pos);
                        panel.panelSet = true;
                    }

                    if (panel.size.x != 0 || panel.size.y != 0 && !panel.panelSet)
                    {
                        ImGui::SetNextWindowSize(panel.size, ImGuiCond_Once);
                        panel.panelSet = true;
                    }
                }

                if (ImGui::Begin(panel.name.c_str(), nullptr, flags))
                {
                    DrawPanelChrome(panel);
                    panel.drawFn();
                }

                ImGui::End();
            }
        }
    };
}