
#ifndef DEMOAPPLICATION_HPP
#define DEMOAPPLICATION_HPP

#include "../ImGui/imgui.h"
#include "../Application/Application.hpp"
#include "../ImGuiCrossPlatform/ImGuiCrossPlatform.hpp"

class DemoApplication : public Application
{
	public:
		bool	x;
		Image	*image;

		DemoApplication(void);

		void Main();

};


#endif // !DEMOAPPLICATION_HPP
