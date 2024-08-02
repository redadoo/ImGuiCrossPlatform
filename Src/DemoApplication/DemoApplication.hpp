#include <filesystem>
#include "ImGuiCrossPlatform.hpp"
#include "Image.hpp"
#include <filesystem>

class DemoApplication : public Application
{
private:
		bool	x = true;
		std::unique_ptr<Image>	image;

        void InitImage();

public:

    DemoApplication();
    ~DemoApplication();
    
    void Main();
    bool IsOpen() const;
};


