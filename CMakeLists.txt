cmake_minimum_required(VERSION 3.20)

project(salamander)
set(CMAKE_CXX_STANDARD 20)

include_directories("src" "vendor/sdl/include" "vendor/imgui")
link_directories("vendor/sdl/lib/x64")

file(GLOB IMGUI_SRC_FILES "vendor/imgui/*.cpp" "vendor/imgui/backends/imgui_impl_sdl.cpp" "vendor/imgui/backends/imgui_impl_sdlrenderer.cpp")
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(salamander ${SRC_FILES} ${IMGUI_SRC_FILES})
target_link_libraries(salamander SDL2main SDL2)