file(GLOB SOURCES *.cpp)

set(LIB_NAME physicsLib)
set(EXE_NAME physim)


add_library(${LIB_NAME} STATIC ${SOURCES})
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/physics)


find_package(SDL2 REQUIRED)
target_include_directories(${LIB_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})


add_executable(${EXE_NAME} main.cpp)
target_include_directories(${EXE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(${EXE_NAME} ${LIB_NAME} SDL2::SDL2)

set_target_properties(${LIB_NAME} ${EXE_NAME} PROPERTIES CXX_STANDARD 20)
