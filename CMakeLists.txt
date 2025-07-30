cmake_minimum_required(VERSION 3.16)
project(salamander LANGUAGES CXX)

# === Settings ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Dependencies ===
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glad)
find_package(OpenGL REQUIRED)

# === Sources ===
add_library(${PROJECT_NAME}
    "src/core/App.cpp"
    "src/core/Input.cpp"
    "src/core/Window.cpp"
    "src/renderer/BatchRenderer.cpp"
    "src/renderer/Buffer.cpp"
    "src/renderer/Camera.cpp"
    "src/renderer/RenderAPI.cpp"
    "src/renderer/Shader.cpp"
    "src/renderer/Texture.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>

    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb>
        $<INSTALL_INTERFACE:vendor/stb>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC glfw glm glad
    PRIVATE OpenGL::GL
)

if (BUILD_EXAMPLES)
    add_subdirectory("examples/Basic")
endif()