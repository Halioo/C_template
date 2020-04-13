cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 99)

# Project variables
set(PROSE_PROJECT_NAME C_template)

project(${PROSE_PROJECT_NAME} C)



add_library(util util.h)
set_target_properties(util PROPERTIES LINKER_LANGUAGE C)
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

target_link_libraries(${PROSE_PROJECT_NAME} util)
