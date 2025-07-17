cmake_minimum_required(VERSION 3.16)

project(glad)

add_library(${PROJECT_NAME}
    "src/glad.c"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)