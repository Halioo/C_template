
file(GLOB_RECURSE everyFile "*/*.c" "*/*.h")

add_executable(${PROSE_PROJECT_NAME}
    main.c
    ${everyFile}
)

target_link_libraries(${PROSE_PROJECT_NAME}
    watchdog mailbox
    pthread rt
)

get_property(loc_LIB_DIR GLOBAL PROPERTY LIB_DIR)
target_include_directories(${PROSE_PROJECT_NAME} PUBLIC ${loc_LIB_DIR})
