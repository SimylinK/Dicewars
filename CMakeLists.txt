cmake_minimum_required(VERSION 3.6)
project(dicewars)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c interface.c interface.h  util.c util.h map.c map.h)
add_executable(dicewars ${SOURCE_FILES})

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})