#include "imguiCrossPlatformWin32.hpp"

ID3D11Device* imguiCrossPlatformWin32::pd3dDevice = nullptr;
ID3D11DeviceContext* imguiCrossPlatformWin32::pd3dDeviceContext = nullptr;
IDXGISwapChain* imguiCrossPlatformWin32::pSwapChain = nullptr;
ID3D11RenderTargetView* imguiCrossPlatformWin32::pMainRenderTargetView = nullptr;

HMODULE imguiCrossPlatformWin32::hCurrentModule = nullptr;

bool imguiCrossPlatformWin32::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    #ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void imguiCrossPlatformWin32::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr)
    {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void imguiCrossPlatformWin32::CleanupRenderTarget()
{
    if (pMainRenderTargetView)
    {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void imguiCrossPlatformWin32::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }

    if (pd3dDeviceContext)
    {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }

    if (pd3dDevice)
    {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

LRESULT WINAPI imguiCrossPlatformWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (imgui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    case WM_DPICHANGED:
        if (imgui::GetIO().ConfigFlags & imguiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void imguiCrossPlatformWin32::Initimgui()
{
    imgui_ImplWin32_EnableDpiAwareness();
    imguiCrossPlatformWin32::wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, _T("imgui Standalone"), nullptr };
    ::RegisterClassEx(&imguiCrossPlatformWin32::wc);
    imguiCrossPlatformWin32::hwnd = ::CreateWindow(imguiCrossPlatformWin32::wc.lpszClassName, _T("imgui Standalone"), WS_OVERLAPPEDWINDOW, 100, 100, 50, 50, NULL, NULL, imguiCrossPlatformWin32::wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    const HMONITOR monitor = MonitorFromWindow(imguiCrossPlatformWin32::hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

    imgui_CHECKVERSION();
    imgui::CreateContext();
    imguiIO& io = imgui::GetIO(); (void)io;
    io.ConfigFlags |= imguiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= imguiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= imguiConfigFlags_DockingEnable;

    imgui::StyleColorsDark();

    imguiStyle& style = imgui::GetStyle();
    if (io.ConfigFlags & imguiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 4.0f;
        style.Colors[imguiCol_WindowBg].w = 1.0f;
    }

    imgui_ImplWin32_Init(hwnd);
    imgui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    if (monitor_height > 1080)
    {
        const float fScale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * fScale;
        imgui::GetIO().Fonts->AddFontDefault(&cfg);
    }

    imgui::GetIO().IniFilename = nullptr;
    ConfFlags = io.ConfigFlags;
}

bool imguiCrossPlatformWin32::ShouldQuit()
{
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            bDone = true;
    }

    if (GetAsyncKeyState(VK_END) & 1)
    {
        bDone = true;
    }

    if (bDone)
        return true;
    return false;
}

void imguiCrossPlatformWin32::Run(Application* app)
{
    imguiCrossPlatformWin32::Initimgui();
    imguiCrossPlatformWin32::Render(app);
    imguiCrossPlatformWin32::CleanUp();
}

void imguiCrossPlatformWin32::Render(Application* app)
{
    while (!bDone)
    {
        if (ShouldQuit() == true || app->IsClosed() == true) break;

        imgui_ImplDX11_NewFrame();
        imgui_ImplWin32_NewFrame();
        
        imgui::NewFrame();
        app->Main();
        imgui::Render();

        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);

        imgui_ImplDX11_RenderDrawData(imgui::GetDrawData());

        if (ConfFlags & imguiConfigFlags_ViewportsEnable)
        {
            imgui::UpdatePlatformWindows();
            imgui::RenderPlatformWindowsDefault();
        }

        pSwapChain->Present(1, 0);

    }
    //delete visualizer;
}

void imguiCrossPlatformWin32::CleanUp()
{
    imgui_ImplDX11_Shutdown();
    imgui_ImplWin32_Shutdown();

    imgui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(imguiCrossPlatformWin32::hwnd);
    ::UnregisterClass(imguiCrossPlatformWin32::wc.lpszClassName, imguiCrossPlatformWin32::wc.hInstance);

#ifdef _WINDLL
    CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, hCurrentModule, NULL, nullptr);
#endif
}