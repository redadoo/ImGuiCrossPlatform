#pragma once

#include "imgui.h"
#include <stdexcept>
#include <iostream>
#include "../ImGui/imgui_internal.h"

namespace ImGuiThemes
{ 

	enum Themes
	{
		None = -1,
		Standart = 0,
		Cinder,
		Adobe,
		Enemymouse,
		olekristensen,
		Dougblinks,
		Light,
		Classic,
		ThemesCount
	};

	Themes IndexToTheme(int index);
	const char* ThemesToString(Themes theme);
	void InitTheme(Themes theme);
}