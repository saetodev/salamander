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
    "src/Window.cpp"
    "src/graphics/BatchRenderer.cpp"
    "src/graphics/Buffer.cpp"
    "src/graphics/Shader.cpp"
    "src/graphics/Texture.cpp"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC glfw glm glad
    PRIVATE OpenGL::GL
)

# === Example Project ===
add_subdirectory(example)