// ImGuiCrossPlatform.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#ifdef __linux__
	#include "../ImGuiCrossPlatformImpl/ImGuiCrossPlatformLinux.hpp"
	using ImageTexture = GLuint;
#elif _WIN32
	#include "../ImGuiCrossPlatformImpl/ImGuiCrossPlatformWin32.hpp"
	using ImageTexture = ID3D11ShaderResourceView*;
#endif 

struct Image
{
	int				myImageHeight;
	int				myImageWidth;
	ImageTexture	texture;

	ImTextureID GetTexture() const;

	void Release();

	Image();
};

class ImGuiCrossPlatfrom
{
	public:
		static void Run(Application *app);
};

