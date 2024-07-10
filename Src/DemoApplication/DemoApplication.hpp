
#ifndef DEMOAPPLICATION_HPP
#define DEMOAPPLICATION_HPP

#include "../../lib/imgui/imgui.h"
#include "../Application/Application.hpp"
#include "../imguiCrossPlatform/imguiCrossPlatform.hpp"

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
