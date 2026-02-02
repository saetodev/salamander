cmake_minimum_required(VERSION 3.16)
project(bunnymark)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} "salamander")
