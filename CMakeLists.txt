cmake_minimum_required(VERSION 3.24)
project(space)
set(CMAKE_CXX_FLAGS "-std=c++14")

# Dependencies
include(FetchContent)
set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILE src/main.cpp
                src/game.cpp
                src/spaceship.cpp
                src/laser.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME} raylib)


