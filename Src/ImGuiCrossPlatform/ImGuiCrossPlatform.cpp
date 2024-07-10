
#include "imguiCrossPlatform.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib/stb_image/stb_image.h"

void imguiCrossPlatfrom::Run(Application *app)
{
	#ifdef __linux__
		imguiCrossPlatformLinux::Run(app);
	#elif _WIN32
		imguiCrossPlatformWin32::Run(app);
	#endif 
}

bool LoadTextureFromFile(const char* filename, Image& image, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
	{
		std::cout << stbi_failure_reason() << "\n";
		return false;
	}

#ifdef __linux__
	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	image.texture = image_texture; 
	*out_width = image_width;
	*out_height = image_height;
#elif _WIN32
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	imguiCrossPlatformWin32::pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;	
	ID3D11ShaderResourceView* tmp = NULL;
	imguiCrossPlatformWin32::pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &tmp);
	image.texture = tmp;
	pTexture->Release();
	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);
#endif

	return true;
}

Image::~Image()
{
	this->Release();
}

Image::Image(const char* _filePath)
{
	this->myImageHeight = 0;
	this->myImageWidth = 0;
	filePath = filePath;
}

ImTextureID Image::GetTexture() const
{
	#ifdef __linux__
		return (void*)(intptr_t)this->texture;
	#elif _WIN32
		return (void*)this->texture;
	#endif
}

void Image::Release()
{
	#ifdef __linux__
		glDeleteTextures(1, &texture);
	#elif _WIN32
		this->texture->Release();
	#endif 
}
