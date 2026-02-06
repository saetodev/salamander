cmake_minimum_required(VERSION 3.16)
project(salamander LANGUAGES CXX)

# === Settings ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Dependencies ===
add_subdirectory("vendor/miniaudio")
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/glad)

# ----------------------------------------
# ANGLE (macOS)
# ----------------------------------------
if (APPLE)
    # Adjust if you built ANGLE somewhere else
    set(ANGLE_ROOT "/opt/local")

    find_library(ANGLE_EGL
        NAMES EGL
        PATHS ${ANGLE_ROOT}/lib
        REQUIRED
    )

    find_library(ANGLE_GLESv2
        NAMES GLESv2
        PATHS ${ANGLE_ROOT}/lib
        REQUIRED
    )

    include_directories(${ANGLE_ROOT}/include)
endif()

# === Sources ===
add_library(${PROJECT_NAME}
    "src/audio/AudioDevice.cpp"
    "src/audio/Sound.cpp"
    "src/core/App.cpp"
    "src/core/Input.cpp"
    "src/core/Window.cpp"
    "src/graphics/gpu.cpp"
    "src/graphics/Renderer2D.cpp"
    "src/graphics/Camera.cpp"
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
    PUBLIC miniaudio glfw glm glad
)

# ----------------------------------------
# Link ANGLE instead of OpenGL
# ----------------------------------------
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${ANGLE_EGL}
            ${ANGLE_GLESv2}
    )
endif()

# === Examples ===
if (BUILD_EXAMPLES)
    add_subdirectory("examples/bunnymark")
    add_subdirectory("examples/triangle")
endif()

