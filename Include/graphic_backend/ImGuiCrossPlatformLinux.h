#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "../BackendFlags.h"
#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
    #include <SDL_opengles2.h>
#else
    #include <SDL2/SDL_opengl.h>
#endif

#include <cstdio>

struct LinuxContext
{
    SDL_Window*      window      = nullptr;
    SDL_GLContext    glContext   = {};
    SDL_WindowFlags  windowFlags = {};
    ImGuiConfigFlags configFlags = 0;
    ImVec4           clearColor  = { 0.45f, 0.55f, 0.60f, 1.00f };
    bool             done        = false;
    UI::BackendFlags backendFlags = UI::BackendFlags::None;
};

struct ImGuiCrossPlatformLinux
{
    LinuxContext ctx;

    inline void SetFlags(UI::BackendFlags flags)
    {
        ctx.backendFlags = flags;
    }

    inline bool Init(const char* windowTitle, int width, int height, int /*fps*/)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            return false;
        }

#if defined(IMGUI_IMPL_OPENGL_ES2)
        const char* glsl_version = "#version 100";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
        const char* glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
        const char* glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

#ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        const bool floating = HasFlag(ctx.backendFlags, UI::BackendFlags::FloatingPanels);
        ctx.windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

        ctx.window = SDL_CreateWindow(
            windowTitle,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            floating ? 1 : width,
            floating ? 1 : height,
            ctx.windowFlags
        );

        if (!ctx.window)
        {
            printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
            return false;
        }

        if (floating)
            SDL_HideWindow(ctx.window);

        ctx.glContext = SDL_GL_CreateContext(ctx.window);
        SDL_GL_MakeCurrent(ctx.window, ctx.glContext);
        SDL_GL_SetSwapInterval(1);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (floating)
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.IniFilename = nullptr;
        ctx.configFlags = io.ConfigFlags;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplSDL2_InitForOpenGL(ctx.window, ctx.glContext);
        ImGui_ImplOpenGL3_Init(glsl_version);
        return true;
    }

    inline void BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    inline void EndFrame() const
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        const float* c = &ctx.clearColor.x;
        glClearColor(c[0] * c[3], c[1] * c[3], c[2] * c[3], c[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (ctx.configFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window*   backup_win = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_ctx = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_win, backup_ctx);
        }

        SDL_GL_SwapWindow(ctx.window);
    }

    inline bool ShouldQuit()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                ctx.done = true;
            if (event.type == SDL_WINDOWEVENT
                && event.window.event == SDL_WINDOWEVENT_CLOSE
                && event.window.windowID == SDL_GetWindowID(ctx.window))
                ctx.done = true;
        }

        if (HasFlag(ctx.backendFlags, UI::BackendFlags::InstantQuitWithEsc))
        {
            const Uint8* keys = SDL_GetKeyboardState(nullptr);
            if (keys[SDL_SCANCODE_ESCAPE])
                ctx.done = true;
        }

        return ctx.done;
    }

    inline void Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_GL_DeleteContext(ctx.glContext);
        SDL_DestroyWindow(ctx.window);
        SDL_Quit();
    }
};