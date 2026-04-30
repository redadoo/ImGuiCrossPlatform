#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../BackendFlags.h"

#include <GLFW/glfw3.h>
#include <cstdio>

struct GLFWContext
{
    GLFWwindow*      window      = nullptr;
    ImGuiConfigFlags configFlags = 0;
    ImVec4           clearColor  = { 0.45f, 0.55f, 0.60f, 1.00f };
    bool             done        = false;
    UI::BackendFlags backendFlags = UI::BackendFlags::None;
};

struct ImGuiCrossPlatformGLFW
{
    GLFWContext ctx;

    inline void SetFlags(UI::BackendFlags flags)
    {
        ctx.backendFlags = flags;
    }

    inline bool Init(const char* windowTitle, int width, int height, int fps)
    {
        if (!glfwInit()) return false;

        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        const bool floating = HasFlag(ctx.backendFlags, UI::BackendFlags::FloatingPanels);

        // If floating mode: create a tiny hidden window (viewports take over)
        ctx.window = glfwCreateWindow(
            floating ? 1 : width,
            floating ? 1 : height,
            windowTitle, nullptr, nullptr
        );
        if (!ctx.window) return false;

        if (floating)
            glfwHideWindow(ctx.window);   // same trick as your Linux backend

        glfwMakeContextCurrent(ctx.window);
        glfwSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (floating)
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // 👈 real OS windows
        io.IniFilename = nullptr;
        ctx.configFlags = io.ConfigFlags;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(ctx.window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        return true;
    }

    inline void BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    inline void EndFrame() const
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(ctx.window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        const float* c = &ctx.clearColor.x;
        glClearColor(c[0]*c[3], c[1]*c[3], c[2]*c[3], c[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // This is where the magic happens — spawns real OS windows
        if (ctx.configFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);  // same pattern as your SDL2 backup
        }

        glfwSwapBuffers(ctx.window);
    }

    inline bool ShouldQuit()
    {
        glfwPollEvents();

        if (glfwWindowShouldClose(ctx.window))
            ctx.done = true;

        if (HasFlag(ctx.backendFlags, UI::BackendFlags::InstantQuitWithEsc))
        {
            if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                ctx.done = true;
        }

        return ctx.done;
    }

    inline void Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(ctx.window);
        glfwTerminate();
    }
};