#
# CMakeLists lib
#
# @author Clément Puybareau
# @copyright CCBY 4.0
#


# Create a global variable for the lib headers directory
set_property(GLOBAL PROPERTY LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Compile CMake for the different libs
add_subdirectory(watchdog)
add_subdirectory(mailbox)

# TODO if you want to add another library :
# Add the following line in this CMakeLists.txt :
    # add_subdirectory(<package_name>)
# Copy an already existing package, rename it and include the .c files.
# Add the .h files in the "include" directory
# To name your newly created library, go to your package and
# modify the "LIB_NAME" variable