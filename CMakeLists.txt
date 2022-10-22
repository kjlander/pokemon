cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# set the project name
project(pokemon LANGUAGES CXX VERSION 1.0)

# set CXX language standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED true)

# set up status messages and default to Release if type not specified
if (CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "STATUS: Build type not specified, set to Release")
    set(CMAKE_BUILD_TYPE Release)
else ()
    message(STATUS "STATUS: Build type specified as ${CMAKE_BUILD_TYPE}")
endif ()

# define _DEBUG or NDEBUG as appropriate
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "_DEBUG")
else ()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
endif ()

# set subdirectories for debug and release binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

# export compile_commands.json (for clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_subdirectory(src)