# test

cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCE_FILES src/*.cc)

add_executable				(test ${SOURCE_FILES})
target_link_libraries		(test geoxide)
target_include_directories	(test PUBLIC include ${geoxide_SOURCE_DIR}/include)

add_custom_command (TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:test>
)

add_custom_command (TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:direct3d11_renderer> $<TARGET_FILE_DIR:test>
)
