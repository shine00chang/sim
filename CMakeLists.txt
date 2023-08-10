cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(physics)

# Add your source files
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/collision.cpp
    src/body.cpp
    src/view.cpp
    src/effects.cpp
)

# Add your header files
set(HEADERS
    src/app.h
    src/environment.h
    src/collision.h
    src/core.h
    src/body.h
    src/testEnv.h
    src/view.h
)

include_directories(SDL2/include)
link_directories(SDL2/lib)

add_executable(prog ${SOURCES} ${HEADERS})
set_property(TARGET prog PROPERTY CXX_STANDARD 20)

target_link_libraries(prog PUBLIC SDL2)
