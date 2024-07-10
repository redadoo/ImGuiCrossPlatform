#include "../ImGuiCrossPlatform/ImGuiCrossPlatform.hpp"
#include "../../lib/imgui/imgui.h"

class DemoApplication : public Application
{
private:
		bool	x = true;
		Image	*image;

public:

    DemoApplication();
    ~DemoApplication();
    
    void Main();
    bool IsOpen() const;
};


