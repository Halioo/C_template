file(GLOB_RECURSE SRC *.c)
get_property(loc_LIB_DIR GLOBAL PROPERTY LIB_DIR)

add_library(watchdog ${SRC})
target_include_directories(watchdog PRIVATE ${loc_LIB_DIR})
set_target_properties(watchdog PROPERTIES LINKER_LANGUAGE C)