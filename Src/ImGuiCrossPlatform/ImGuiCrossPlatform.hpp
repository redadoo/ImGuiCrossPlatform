﻿#pragma once

#ifndef imguiCROSSPLATFORM_HPP

#ifdef __linux__
	#include "../imguiCrossPlatformImpl/imguiCrossPlatformLinux.hpp"
	using ImageTexture = GLuint;
#elif _WIN32
	#include "../imguiCrossPlatformImpl/imguiCrossPlatformWin32.hpp"
	using ImageTexture = ID3D11ShaderResourceView*;
#endif

#include <filesystem>

struct Image
{
	std::filesystem::path		filePath;
	ImageTexture				texture;

	int							myImageHeight;
	int							myImageWidth;


	Image(const char* imagePath);
	~Image();

	ImTextureID GetTexture() const;
	void Release();
};

class imguiCrossPlatfrom
{
public:
	static void Run(Application* app);
};

bool LoadTextureFromFile(const char* filename, Image& image, int* out_width, int* out_height);


#endif // !imguiCROSSPLATFORM_HPP



