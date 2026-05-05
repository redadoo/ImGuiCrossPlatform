cmake_minimum_required(VERSION 3.22)
project(ImGuiCrossPlatform LANGUAGES CXX)

option(USE_RAYLIB "Use Raylib backend" OFF)

message(STATUS "ImGuiCrossPlatform: USE_RAYLIB=${USE_RAYLIB}")

include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/imgui.cmake)

# ===== Backend setup =====
if(USE_RAYLIB)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/raylib.cmake)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/rlimgui.cmake)

elseif(UNIX)
    target_sources(imgui PRIVATE
            ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    target_link_libraries(imgui PRIVATE SDL2::SDL2 OpenGL::GL)

elseif(WIN32)
    target_sources(imgui PRIVATE
            ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    )

else()
    message(FATAL_ERROR "ImGuiCrossPlatform: unsupported platform")
endif()

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# ===== Library =====
add_library(ImGuiCrossPlatform INTERFACE)
add_library(ImGuiCrossPlatform::ImGuiCrossPlatform ALIAS ImGuiCrossPlatform)

target_compile_features(ImGuiCrossPlatform INTERFACE cxx_std_20)

target_include_directories(ImGuiCrossPlatform INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

if(USE_RAYLIB)
    target_compile_definitions(ImGuiCrossPlatform INTERFACE USE_RAYLIB)
    target_link_libraries(ImGuiCrossPlatform INTERFACE imgui raylib rlimgui)

elseif(UNIX)
    target_link_libraries(ImGuiCrossPlatform INTERFACE imgui)

elseif(WIN32)
    target_link_libraries(ImGuiCrossPlatform INTERFACE imgui d3d11 dxgi)
endif()