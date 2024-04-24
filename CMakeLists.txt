cmake_minimum_required(VERSION 3.24)
project(space)
set(CMAKE_CXX_FLAGS "-std=c++14")

# Dependencies
include(FetchContent)
# raylib
set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)


# yaml-cpp
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)
set(SOURCE_FILE src/main.cpp
                src/Game.cpp
                src/SpaceShip.cpp
                src/Laser.cpp
                src/Block.cpp
                src/Obstacle.cpp
                src/Alien.cpp
                src/MysteryShip.cpp
                src/MainMenu.cpp
                src/OptionList.cpp
                src/ButtonHandler.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})
target_link_libraries(${PROJECT_NAME} raylib yaml-cpp::yaml-cpp)