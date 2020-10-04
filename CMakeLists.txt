cmake_minimum_required(VERSION 3.16)
project(audiorex C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB AUDIOREX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
 )

add_library(audiorex SHARED ${AUDIOREX_SOURCES})
add_library(AudioRex::audiorex ALIAS audiorex)