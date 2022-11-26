# test

cmake_minimum_required(VERSION 3.16)

file(GLOB SHADER_FILES assets/shaders/*.fx)
file(GLOB SOURCE_FILES src/*.cc)

add_executable				(test ${SOURCE_FILES})
add_dependencies			(test direct3d11_renderer)
target_link_libraries		(test geoxide)
target_include_directories	(test PUBLIC include ${geoxide_SOURCE_DIR}/include)

install(FILES ${SHADER_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/shaders)

install(TARGETS test
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
