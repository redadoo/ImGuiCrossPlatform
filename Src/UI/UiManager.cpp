#include "UiManager.h"
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

void UIManager::SetBackendFlags(BackendFlags flags)
{
	Visit([flags](auto& b) { b.SetFlags(flags); });

}

void UIManager::StartDrawScene()
{
	Visit([](auto& b) { b.BeginFrame(); });
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