cmake_minimum_required(VERSION 3.22.1)
set (CMAKE_CXX_STANDARD 17)
project(ImGuiCrossPlatform)
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
    ${IMGUI_PATH}/imgui_widgets.cpp

    if (UNIX)
        ${IMGUI_PATH}/imgui_impl_sdl2.h
        ${IMGUI_PATH}/imgui_impl_sdl2.cpp
        ${IMGUI_PATH}/imgui_impl_opengl3.h
        ${IMGUI_PATH}/imgui_impl_opengl3.cpp
    endif (UNIX)

    )

add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})

## Find dependencies
if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
endif (UNIX)

if (UNIX)
    add_executable(ImGuiCrossPlatform "Src/Main/ImGuiCrossPlatform.cpp" "Src/ImGuiCrossPlatform/ImGuiCrossPlatformLinux.cpp")
endif (UNIX)
if (WIN32)
    add_executable(ImGuiCrossPlatform WIN32 "Src/Main/ImGuiCrossPlatform.cpp" "Src/ImGuiCrossPlatform/ImGuiCrossPlatformWin32.cpp")
endif (WIN32)


if (UNIX)
    target_link_libraries(ImGuiCrossPlatform PUBLIC ImGui ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
endif (UNIX)
if (WIN32)
    target_link_libraries(ImGuiCrossPlatform PUBLIC ImGui)
endif (WIN32)