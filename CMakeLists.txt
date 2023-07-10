cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(physics)

include_directories(SDL2/include)
link_directories(SDL2/lib)

add_executable(prog src/main.cpp)

target_link_libraries(prog PUBLIC SDL2)
