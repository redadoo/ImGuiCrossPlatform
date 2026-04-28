cmake_minimum_required(VERSION 3.22.1)
project(ImGuiCrossPlatform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# ===== Options =====
option(USE_RAYLIB "Use Raylib as the UI backend" OFF)

# General compile options (GCC/Clang only)
if(NOT MSVC)
    add_compile_options(-Wall -Wextra -Werror)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3)
    endif()
endif()

# ===== Dependencies =====
include(FetchContent)

include(cmake/imgui.cmake)

if(USE_RAYLIB)
    include(cmake/raylib.cmake)
    include(cmake/rlimgui.cmake)
else()
    # Append platform-specific ImGui backend sources
    if(UNIX)
        target_sources(imgui PRIVATE
                ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        )
    elseif(WIN32)
        target_sources(imgui PRIVATE
                ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        )
    endif()
endif()

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# ===== Source files =====
# All backends are header-only — no .cpp files needed per backend
set(SOURCES
        Src/Main/main.cpp
        Src/UI/UiManager.cpp
)

# ===== Executable =====
if(WIN32 AND NOT USE_RAYLIB)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

# ===== Compile definitions =====
if(USE_RAYLIB)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_RAYLIB)
endif()

# ===== Include directories =====
target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/Src
        ${PROJECT_SOURCE_DIR}/Include
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# ===== Link libraries =====
if(USE_RAYLIB)
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui rlimgui raylib)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
    if(UNIX)
        find_package(OpenGL REQUIRED)
        find_package(SDL2 REQUIRED)

        # Link SDL2 + OpenGL to imgui (THIS is the important fix)
        target_link_libraries(imgui PRIVATE SDL2::SDL2 OpenGL::GL)

        # Your executable can still link imgui
        target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
    endif()
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)