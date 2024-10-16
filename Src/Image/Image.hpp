#pragma once

#ifndef IMAGE_HPP
#define IMAGE_HPP


#ifdef __linux__
	#include "ImGuiCrossPlatformLinux.hpp"	
	using ImageTexture = GLuint;
	#define NULL_ 0
#elif _WIN32
	#include "ImGuiCrossPlatformWin32.hpp"
	using ImageTexture = ID3D11ShaderResourceView*;
	#define NULL_ NULL
#endif

struct Image
{
	char*			filePath;
	ImageTexture	texture;

	int				myImageHeight;
	int				myImageWidth;

	Image();
	Image(const char* imagePath);
	Image(const std::string& imagePath);

	~Image();

	ImTextureID GetTexture() const;
	void Release();
};

bool LoadTextureFromFile(const char* filename, Image& image);
bool LoadTextureFromFile(const char* filename, std::unique_ptr<Image> &image);

#endif