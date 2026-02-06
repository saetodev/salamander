cmake_minimum_required(VERSION 3.16)
project(triangle)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} "salamander")

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "/opt/local/lib"
        INSTALL_RPATH "/opt/local/lib"
    )
endif()