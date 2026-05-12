#pragma comment(lib, "d3d11.lib")
#pragma once

#include <Windows.h>
#include <tchar.h>
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "BackendFlags.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct Win32Context
{
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* renderTargetView = nullptr;
    HWND hwnd = nullptr;
    WNDCLASSEX wc = {};
    HMODULE hModule = nullptr;
    ImGuiConfigFlags configFlags      = 0;
    ImVec4 clearColor = { 0.45f, 0.55f, 0.60f, 1.00f };
    int minWidth  = 0;
    int minHeight = 0;
    int maxWidth  = 0;
    int maxHeight = 0;
    bool done = false;
    bool floatingMode = false;
    UI::BackendFlags backendFlags = UI::BackendFlags::None;
};

struct ImGuiCrossPlatformWin32
{
    Win32Context ctx;

    inline void SetFlags(UI::BackendFlags flags)
    {
        ctx.backendFlags = flags;
    }

    inline void SetMinWindowSize(const int h, const int w)
    {
        ctx.minWidth  = w;
        ctx.minHeight = h;
    }

    inline void SetMaxWindowSize(const int h, const int w)
    {
        ctx.maxWidth  = w;
        ctx.maxHeight = h;
    }

    inline void MaximizeWindow() { ::ShowWindow(ctx.hwnd, SW_MAXIMIZE); }

    inline void MinimizeWindow() { ::ShowWindow(ctx.hwnd, SW_MINIMIZE); }

    inline ImVec2 GetDesktopSize() const
    {
        return ImVec2(
            (float)::GetSystemMetrics(SM_CXSCREEN),
            (float)::GetSystemMetrics(SM_CYSCREEN)
        );
    }

    inline bool IsMaximized() const
    {
        return ::IsZoomed(ctx.hwnd);
    }

    inline void RestoreWindow()
    {
        ::ShowWindow(ctx.hwnd, SW_RESTORE);
    }

    inline void RequestQuit() { ctx.done = true; }

    inline bool Init(const char* windowTitle, int width, int height, int fps)
    {
        ctx.floatingMode = HasFlag(ctx.backendFlags, UI::BackendFlags::FloatingPanels);

        ImGui_ImplWin32_EnableDpiAwareness();

        ctx.wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                   GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                   _T("ImGuiWindow"), nullptr };
        ::RegisterClassEx(&ctx.wc);

        ctx.hwnd = ::CreateWindow(
            ctx.wc.lpszClassName, _T(windowTitle), WS_OVERLAPPEDWINDOW,
            100, 100,
            ctx.floatingMode ? 1 : width,
            ctx.floatingMode ? 1 : height,
            nullptr, nullptr, ctx.wc.hInstance, this
        );

        if (!CreateDeviceD3D(fps))
        {
            CleanupDeviceD3D();
            ::UnregisterClass(ctx.wc.lpszClassName, ctx.wc.hInstance);
            return false;
        }

        ::ShowWindow(ctx.hwnd, ctx.floatingMode ? SW_HIDE : SW_SHOW);
        ::UpdateWindow(ctx.hwnd);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (ctx.floatingMode)
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.IniFilename = nullptr;
        ctx.configFlags = io.ConfigFlags;

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 4.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplWin32_Init(ctx.hwnd);
        ImGui_ImplDX11_Init(ctx.device, ctx.deviceContext);
        return true;
    }

    inline void BeginFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    inline void EndFrame() const
    {
        ImGui::Render();

        const float cc[4] = {
            ctx.clearColor.x * ctx.clearColor.w,
            ctx.clearColor.y * ctx.clearColor.w,
            ctx.clearColor.z * ctx.clearColor.w,
            ctx.clearColor.w
        };
        ctx.deviceContext->OMSetRenderTargets(1, &ctx.renderTargetView, nullptr);
        ctx.deviceContext->ClearRenderTargetView(ctx.renderTargetView, cc);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (ctx.configFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        ctx.swapChain->Present(1, 0);
    }

    inline void Shutdown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
        ::DestroyWindow(ctx.hwnd);
        ::UnregisterClass(ctx.wc.lpszClassName, ctx.wc.hInstance);
    }

    inline bool ShouldQuit()
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                ctx.done = true;
        }

        if (HasFlag(ctx.backendFlags, UI::BackendFlags::InstantQuitWithEsc) && GetAsyncKeyState(VK_ESCAPE) & 1)
            ctx.done = true;

        return ctx.done;
    }

private:
    inline bool CreateDeviceD3D(int fps)
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount                        = 2;
        sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator   = fps;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow                       = ctx.hwnd;
        sd.SampleDesc.Count                   = 1;
        sd.Windowed                           = TRUE;
        sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
#ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        constexpr D3D_FEATURE_LEVEL featureLevelArray[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        D3D_FEATURE_LEVEL featureLevel;

        if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                          createDeviceFlags, featureLevelArray, 2,
                                          D3D11_SDK_VERSION, &sd, &ctx.swapChain,
                                          &ctx.device, &featureLevel, &ctx.deviceContext) != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    inline void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer = nullptr;
        ctx.swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (pBackBuffer)
        {
            ctx.device->CreateRenderTargetView(pBackBuffer, nullptr, &ctx.renderTargetView);
            pBackBuffer->Release();
        }
    }

    inline void CleanupRenderTarget()
    {
        if (ctx.renderTargetView) { ctx.renderTargetView->Release(); ctx.renderTargetView = nullptr; }
    }

    inline void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (ctx.swapChain)     { ctx.swapChain->Release();     ctx.swapChain     = nullptr; }
        if (ctx.deviceContext) { ctx.deviceContext->Release(); ctx.deviceContext  = nullptr; }
        if (ctx.device)        { ctx.device->Release();        ctx.device         = nullptr; }
    }

    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        ImGuiCrossPlatformWin32* self = nullptr;
        if (msg == WM_NCCREATE)
        {
            auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            self = reinterpret_cast<ImGuiCrossPlatformWin32*>(cs->lpCreateParams);
            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        }
        else
        {
            self = reinterpret_cast<ImGuiCrossPlatformWin32*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
            case WM_GETMINMAXINFO:
            {
                if (self)
                {
                    MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);

                    // --- MIN SIZE ---
                    if (self->ctx.minWidth > 0 && self->ctx.minHeight > 0)
                    {
                        RECT rc = { 0, 0, self->ctx.minWidth, self->ctx.minHeight };
                        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

                        mmi->ptMinTrackSize.x = rc.right - rc.left;
                        mmi->ptMinTrackSize.y = rc.bottom - rc.top;
                    }

                    // --- MAX SIZE ---
                    if (self->ctx.maxWidth > 0 && self->ctx.maxHeight > 0)
                    {
                        RECT rc = { 0, 0, self->ctx.maxWidth, self->ctx.maxHeight };
                        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

                        mmi->ptMaxTrackSize.x = rc.right - rc.left;
                        mmi->ptMaxTrackSize.y = rc.bottom - rc.top;
                    }
                }
                return 0;
            }

            case WM_SIZE:
                if (self && self->ctx.device && wParam != SIZE_MINIMIZED)
                {
                    self->CleanupRenderTarget();
                    self->ctx.swapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                    self->CreateRenderTarget();
                }
                return 0;
            case WM_SYSCOMMAND:
                if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
                break;
            case WM_KEYDOWN:
                if (wParam == VK_ESCAPE) { ::PostQuitMessage(0); return 0; }
                break;
            case WM_DESTROY:
                ::PostQuitMessage(0);
                    return 0;
            default:
                break;
        }
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
};