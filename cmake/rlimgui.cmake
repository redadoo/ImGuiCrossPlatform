# Always fetch the latest rlImGui (now compatible with latest imgui + raylib)
FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG        main
        GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(rlimgui)

add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_link_libraries(rlimgui PUBLIC raylib imgui)
target_include_directories(rlimgui INTERFACE ${rlimgui_SOURCE_DIR})