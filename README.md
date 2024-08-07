# imguiCrossPlatform

![GitHub repo size](https://img.shields.io/github/repo-size/redadoo/imguiCrossPlatform)
![GitHub contributors](https://img.shields.io/github/contributors/redadoo/imguiCrossPlatform)
![GitHub stars](https://img.shields.io/github/stars/redadoo/imguiCrossPlatform?style=social)
![GitHub forks](https://img.shields.io/github/forks/redadoo/imguiCrossPlatform?style=social)

## Table of Contents

- [About](#about)
- [Features](#Features)
- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Ressources](#ressources)

## About

imguiCrossPlatform is a project aimed at facilitating cross compilation for the [imgui](https://github.com/ocornut/imgui) library on windows and linux using cmake.

## Features

*  Cross-compile linux(opengl3,sdl2)/Windows(d3d11) imgui library
*  Demo application
* Cross-compile image struct to use in imgui

## Prerequisites
* Linux:
  ```bash
  * cmake
  * sdl2
  * opengl3
  ```
* Windows:
  ```bash
  * cmake
  * visual studio
  * DirectX11 
  ```
## Usage

To utilize `imguiCrossPlatform`
* Linux:
1) Clone the repository:
```bash
git clone https://github.com/redadoo/imguiCrossPlatform.git
```
2) Navigate to the imguiCrossPlatform directory, create build directory, and navigate into the build directory:
```bash
cd imguiCrossPlatform/
mkdir build
cd build
```
3) Generate build
```bash
cmake ..
make
```
4)
```bash
./imguiCrossPlatform
```
* Windows:
1) Clone the repository:
```bash
git clone https://github.com/redadoo/imguiCrossPlatform.git
```
2) Open folder with visual studio
3) create build directory
4) Generate build
```bash
cd build
cmake ..
```
5) select imguiCrossPlatform.exe on visual studio for start up item and press the button

   
![Visual Studio cmake](https://github.com/redadoo/imguiCrossPlatform/assets/23256144/4b5e81b7-f9c3-4dc2-b078-9c50ca79d4aa)
   
## Ressources

[imgui](https://github.com/ocornut/imgui) is a popular graphical user interface library written in C++.

[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) image loading/decoding from file/memory: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC
  
