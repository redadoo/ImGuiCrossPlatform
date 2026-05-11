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
    struct Panel
    {
        std::string name;
        std::function<void()> drawFn;
        ImGuiWindowFlags flags = 0;
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

        void AddPanel(std::string name, std::function<void()> drawFn, const ImGuiWindowFlags flags = 0)
        {
            m_panels.push_back({ std::move(name), std::move(drawFn), flags });
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

    private:
        BackendVariant backend;
        std::vector<Panel> m_panels;
        std::function<void(ImGuiID)> m_layoutFn;
        bool m_layoutInitialized = false;
        bool m_initialized = false;

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

            for (const auto& panel : m_panels)
            {
                if (ImGui::Begin(panel.name.c_str(), nullptr, panel.flags))
                    panel.drawFn();

                ImGui::End();
            }
        }
    };
}