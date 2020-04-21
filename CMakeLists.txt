#
# CMakeLists principal
#
# @author Clément Puybareau
# @copyright CCBY 4.0
#


# Standard values for CMake
cmake_minimum_required(VERSION 3.9)
set(CMAKE_C_STANDARD 99)

# Project variables
set(PROSE_PROJECT_NAME C_template)
project(${PROSE_PROJECT_NAME} C)

# Compile CMake for the lib and src package
add_subdirectory(lib)
add_subdirectory(src)

