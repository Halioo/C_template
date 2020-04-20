file(GLOB_RECURSE SRC *.c)
get_property(loc_LIB_DIR GLOBAL PROPERTY LIB_DIR)

add_library(mailbox ${SRC})
target_include_directories(mailbox PRIVATE ${loc_LIB_DIR})
set_target_properties(mailbox PROPERTIES LINKER_LANGUAGE C)