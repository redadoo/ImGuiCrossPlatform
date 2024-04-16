#pragma once

#ifndef IMGUICROSSPLATFORM_HPP

#ifdef __linux__
	#include "../ImGuiCrossPlatformImpl/ImGuiCrossPlatformLinux.hpp"
	using ImageTexture = GLuint;
#elif _WIN32
	#include "../ImGuiCrossPlatformImpl/ImGuiCrossPlatformWin32.hpp"
	using ImageTexture = ID3D11ShaderResourceView*;
#endif

#include <filesystem>

struct Image
{
	int							myImageHeight;
	int							myImageWidth;
	std::filesystem::path		filePath;

	ImageTexture				texture;

	Image(const char* imagePath);
	~Image();

	ImTextureID GetTexture() const;
	void Release();
};

class ImGuiCrossPlatfrom
{
public:
	static void Run(Application* app);
};

bool LoadTextureFromFile(const char* filename, Image& image, int* out_width, int* out_height);


#endif // !IMGUICROSSPLATFORM_HPP



