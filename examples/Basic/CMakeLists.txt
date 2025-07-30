cmake_minimum_required(VERSION 3.16)
project(salamander-example-basic)

add_executable(${PROJECT_NAME}
    "src/main.cpp"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE "salamander"
)