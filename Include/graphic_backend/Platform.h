#pragma once

#include <cstdio>
#include <cstdlib>

// ─── Platform entry point ─────────────────────────────────────────────────────

#if defined(_WIN32) && !defined(USE_RAYLIB)
    #define PLATFORM_MAIN \
        int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
    #define PLATFORM_MAIN int main()
#endif

// ─── Assert / Warn ───────────────────────────────────────────────────────────

#if defined(USE_RAYLIB)
    // Raylib has its own logging + avoids Windows.h conflicts
    #include "raylib.h"

    #define UI_ASSERT(condition, msg)                                          \
        do {                                                                   \
            if (!(condition)) {                                                \
                TraceLog(LOG_FATAL, "[ASSERT] %s | %s | %s:%d",               \
                    msg, #condition, __FILE__, __LINE__);                      \
                std::abort();                                                  \
            }                                                                  \
        } while(0)

    #define UI_WARN(msg) \
        TraceLog(LOG_WARNING, "[WARN] %s", msg)

#elif defined(_WIN32)
    #include <Windows.h>

    #define UI_ASSERT(condition, msg)                                          \
        do {                                                                   \
            if (!(condition)) {                                                \
                char _buf[512];                                                \
                snprintf(_buf, sizeof(_buf),                                   \
                    "Assertion failed!\n\n"                                    \
                    "Condition : %s\n"                                         \
                    "Message   : %s\n\n"                                       \
                    "File : %s\n"                                              \
                    "Line : %d",                                               \
                    #condition, msg, __FILE__, __LINE__);                      \
                MessageBoxA(nullptr, _buf, "UIManager Error", MB_OK | MB_ICONERROR); \
                std::abort();                                                  \
            }                                                                  \
        } while(0)

    #define UI_WARN(msg) \
        MessageBoxA(nullptr, msg, "UIManager Warning", MB_OK | MB_ICONWARNING)

#else
    // Linux / other — stderr
    #define UI_ASSERT(condition, msg)                                          \
        do {                                                                   \
            if (!(condition)) {                                                \
                fprintf(stderr,                                                \
                    "[ASSERT] %s\nCondition: %s\nFile: %s  Line: %d\n",       \
                    msg, #condition, __FILE__, __LINE__);                      \
                std::abort();                                                  \
            }                                                                  \
        } while(0)

    #define UI_WARN(msg) \
        fprintf(stderr, "[WARN] %s\n", msg)
#endif

// ─── Console ─────────────────────────────────────────────────────────────────

#if defined(_WIN32) && !defined(USE_RAYLIB)
    inline void CreateConsole()
    {
        FILE* fDummy;
        AllocConsole();
        freopen_s(&fDummy, "CONIN$",  "r", stdin);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
    }
#else
    inline void CreateConsole() {}  // raylib handles its own log output; Linux has a terminal
#endif