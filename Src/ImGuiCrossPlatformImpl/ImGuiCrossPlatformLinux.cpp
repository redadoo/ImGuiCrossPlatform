
#include "imguiCrossPlatformLinux.hpp"

SDL_WindowFlags imguiCrossPlatformLinux::windowFlags;
SDL_Window* imguiCrossPlatformLinux::window;
SDL_GLContext	imguiCrossPlatformLinux::glContext;
bool			imguiCrossPlatformLinux::done;

void imguiCrossPlatformLinux::InitimguiCrossPlatformLinux()
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return ;
	}

	// Decide GL+GLSL versions
#if defined(imgui_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("Dear imgui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
	if (imguiCrossPlatformLinux::window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return;
	}

	glContext = SDL_GL_CreateContext(imguiCrossPlatformLinux::window);
	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Setup Dear imgui context
	imgui_CHECKVERSION();
	imgui::CreateContext();
	imguiIO& io = imgui::GetIO(); (void)io;
	io.ConfigFlags |= imguiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= imguiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= imguiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= imguiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	imgui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	imguiStyle& style = imgui::GetStyle();
	if (io.ConfigFlags & imguiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[imguiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	imgui_ImplSDL2_InitForOpenGL(window, glContext);
	imgui_ImplOpenGL3_Init(glsl_version);
}

bool imguiCrossPlatformLinux::ShouldQuit()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		imgui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			return true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			return true;
	}
}

void imguiCrossPlatformLinux::Render(Application* app)
{
	imguiIO& io = imgui::GetIO(); (void)io;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!done)
	{
		if (ShouldQuit() == true || app->IsClosed() == true) break;
		
		imgui_ImplOpenGL3_NewFrame();
		imgui_ImplSDL2_NewFrame();
		imgui::NewFrame();
		app->Main();
		imgui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		imgui_ImplOpenGL3_RenderDrawData(imgui::GetDrawData());

		if (io.ConfigFlags & imguiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			imgui::UpdatePlatformWindows();
			imgui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_GL_SwapWindow(window);
	}
}

void imguiCrossPlatformLinux::CleanUp()
{
	imgui_ImplOpenGL3_Shutdown();
	imgui_ImplSDL2_Shutdown();
	imgui::DestroyContext();

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void imguiCrossPlatformLinux::Run(Application* app)
{
	InitimguiCrossPlatformLinux();
	Render(app);
	CleanUp(); 
}