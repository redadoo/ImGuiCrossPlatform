#pragma once

#if defined(_WIN32) && !defined(USE_RAYLIB)
	#include <Windows.h>
	#define PLATFORM_MAIN \
	int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
	#define PLATFORM_MAIN int main()
#endif