#
# CMakeLists src
#
# @author Clément Puybareau
# @copyright CCBY 4.0
#

# store all .c and .h files in an "everyFile" variable
file(GLOB_RECURSE everyFile "*/*.c" "*/*.h")
# To add another directory level, add : "*/*/*.c" "*/*/*.h"

# Create an executable with a main.c and everyFile
# To add another application, add another "add_executable"
# method with a different name
add_executable(${PROSE_PROJECT_NAME}
    main.c
    ${everyFile}
)

# List the different librairies used by the application
# To add another library, just add its name to the list
target_link_libraries(${PROSE_PROJECT_NAME}
    pthread rt
    watchdog mailbox
)

# Add a header directory to search in
# The "LIB_DIR" global property is set in lib/CMakeLists.txt
get_property(loc_LIB_DIR GLOBAL PROPERTY LIB_DIR)
target_include_directories(${PROSE_PROJECT_NAME} PUBLIC ${loc_LIB_DIR})
