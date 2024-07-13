#pragma once

#ifndef IMGUICROSSPLATFORM_HPP


#ifdef __linux__
	#include "ImGuiCrossPlatformLinux.hpp"
	using ImageTexture = GLuint;
#elif _WIN32
	#include "ImGuiCrossPlatformWin32.hpp"
	using ImageTexture = ID3D11ShaderResourceView*;
#endif


struct Image
{
	char*			filePath;
	ImageTexture	texture;

	int				myImageHeight;
	int				myImageWidth;


	Image(const char* imagePath);
	Image(const std::string &imagePath);

	~Image();

	ImTextureID GetTexture() const;
	void Release();
};

class ImGuiCrossPlatfrom
{
public:
	static void Run(Application* app);
};

bool LoadTextureFromFile(const char* filename, Image& image);


#endif // !IMGUICROSSPLATFORM_HPP



