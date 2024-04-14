cmake_minimum_required(VERSION 3.22.1)
project(ImGuiCrossPlatform)

set (CMAKE_CXX_STANDARD 17)
set(IMGUI_PATH  "Src/ImGui")

# Compile as static library 
file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp)


if (UNIX)
    file(GLOB IMGUI_BACKEND_SOURCES
    ${IMGUI_PATH}/imgui_impl_sdl2.h
    ${IMGUI_PATH}/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/imgui_impl_opengl3.h
    ${IMGUI_PATH}/imgui_impl_opengl3.cpp)
elseif (WIN32)
    file(GLOB IMGUI_BACKEND_SOURCES
    ${IMGUI_PATH}/imgui_impl_dx11.h
    ${IMGUI_PATH}/imgui_impl_dx11.cpp
    ${IMGUI_PATH}/imgui_impl_win32.h
    ${IMGUI_PATH}/imgui_impl_win32.cpp)
endif (WIN32)

add_library("ImGui" STATIC ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})

if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
endif (UNIX)

if (UNIX)
    add_executable(ImGuiCrossPlatform "Src/Main/main.cpp" "Src/ImGuiCrossPlatform/ImGuiCrossPlatform.cpp" "Src/ImGuiCrossPlatformImpl/ImGuiCrossPlatformLinux.cpp")
elseif (WIN32)
    add_executable(ImGuiCrossPlatform WIN32 "Src/Main/main.cpp" "Src/ImGuiCrossPlatform/ImGuiCrossPlatform.cpp" "Src/ImGuiCrossPlatformImpl/ImGuiCrossPlatformWin32.cpp")
endif (WIN32)


if (UNIX)
    target_link_libraries(ImGuiCrossPlatform PUBLIC ImGui ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
elseif (WIN32)
    target_link_libraries(ImGuiCrossPlatform PUBLIC ImGui)
endif (WIN32)
