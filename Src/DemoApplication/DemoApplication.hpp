
#ifndef DEMOAPPLICATION_HPP
#define DEMOAPPLICATION_HPP

#include "../ImGui/imgui.h"
#include "../Application/Application.hpp"
#include "../ImGuiCrossPlatform/ImGuiCrossPlatform.hpp"

class DemoApplication : public Application
{
	private:
		bool	x;
		Image	*image;

	public:

		DemoApplication(void);

		void Main();
		bool IsClosed();

};


#endif // !DEMOAPPLICATION_HPP
