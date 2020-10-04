cmake_minimum_required(VERSION 3.16)
project(audiorex C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(BUILD_EXAMPLES "Build examples for AudioRex library." ON)
option(BUILD_AUDIOREX_STATIC "Build AudioRex library in static mode." ON)

file(GLOB AUDIOREX_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
 )

if (BUILD_AUDIOREX_STATIC)
    add_library(audiorex STATIC ${AUDIOREX_SOURCES})
else()
    add_library(audiorex SHARED ${AUDIOREX_SOURCES})
    target_compile_definitions(audiorex PUBLIC DLL_PLATFORM)
    target_compile_definitions(audiorex PUBLIC LIB_EXPORTS)
endif()

target_include_directories(audiorex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(AudioRex::audiorex ALIAS audiorex)

if (BUILD_EXAMPLES)
    file(GLOB SIMPLE_PLAYBACK_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_playback/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_playback/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_playback/*.h" 
    )

    add_executable(simple_playback ${SIMPLE_PLAYBACK_SOURCES})
    target_link_libraries(simple_playback audiorex)
endif()