cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 20)

project(physics)

# Add your source files
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/collision.cpp
    src/renderer.cpp
)

# Add your header files
set(HEADERS
    src/app.h
    src/environment.h
)

include_directories(SDL2/include)
link_directories(SDL2/lib)

add_executable(prog ${SOURCES} ${HEADERS})

target_link_libraries(prog PUBLIC SDL2)
