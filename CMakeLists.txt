cmake_minimum_required(VERSION 3.22.1)
project(imguiCrossPlatform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# General compile options
add_compile_options(-Wall)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
endif()

# Paths
set(IMGUI_PATH "lib/imgui")
set(STB_IMAGE_PATH "lib/stb_image/stb_image.h")

# ImGui sources
set(IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
)

# ImGui backend sources
if (UNIX)
    set(IMGUI_BACKEND_SOURCES
        ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    )
elseif (WIN32)
    set(IMGUI_BACKEND_SOURCES
        ${IMGUI_PATH}/backends/imgui_impl_dx11.cpp
        ${IMGUI_PATH}/backends/imgui_impl_win32.cpp
    )
endif()

# Add ImGui library
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES} ${STB_IMAGE_PATH})
target_include_directories(imgui PUBLIC ${IMGUI_PATH})

# Executable and platform-specific settings
if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)

    include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
    
    add_executable(ImGuiCrossPlatform 
        Src/Main/main.cpp 
        Src/ImGuiCrossPlatform/ImGuiCrossPlatform.cpp 
        Src/ImGuiCrossPlatformImpl/ImGuiCrossPlatformLinux.cpp
        Src/DemoApplication/DemoApplication.cpp
    )
    target_link_libraries(ImGuiCrossPlatform PUBLIC imgui ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})

elseif (WIN32)
    add_executable(imguiCrossPlatform WIN32 
        Src/Main/main.cpp 
        Src/ImGuiCrossPlatform/ImGuiCrossPlatform.cpp 
        Src/ImGuiCrossPlatformImpl/ImGuiCrossPlatformWin32.cpp
        Src/DemoApplication/DemoApplication.cpp
    )
    target_link_libraries(imguiCrossPlatform PUBLIC imgui)
endif()
