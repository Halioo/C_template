file(GLOB_RECURSE everyFile "*/*.c" "*/*.h")

add_executable(${PROSE_PROJECT_NAME}
        main.c ${everyFile})

