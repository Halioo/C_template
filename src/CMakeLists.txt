file(GLOB_RECURSE everyFile "*/*.c" "*/*.h")

add_executable(${PROSE_PROJECT_NAME}
        main.c ${everyFile})

target_link_libraries(${PROSE_PROJECT_NAME} util pthread rt)