#
# CMakeLists mailbox
#
# @author Clément Puybareau
# @copyright CCBY 4.0
#

# TODO : if you create a new lib, change the name here
set(LIB_NAME mailbox)

# Select every .c files of the current directory
file(GLOB_RECURSE SRC *.c)

# Retrieve the header directory
get_property(loc_LIB_DIR GLOBAL PROPERTY LIB_DIR)

# Create the static library
add_library(${LIB_NAME} ${SRC})
target_link_libraries(${LIB_NAME} rt)
target_include_directories(${LIB_NAME} PRIVATE ${loc_LIB_DIR})
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE C)