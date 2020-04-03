add_executable(terra_noise examples/noise.cpp)
target_link_libraries(terra_noise PRIVATE Terra)

add_custom_target(terra_examples DEPENDS Terra terra_noise)

# copy resource files to target directory
add_custom_command(
    TARGET terra_examples PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/resources
        $<TARGET_FILE_DIR:Terra>/resources/)

# additional command to copy kernels into a working directory allowing
# debugging directly from VS
if(MSVC)
    add_custom_command(
        TARGET terra_examples PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/resources
            resources)
endif()
