# d3d11 renderer

cmake_minimum_required(VERSION 3.16)

project(direct3d11_renderer)

file(GLOB SOURCE_FILES src/*.cc)

add_library					(direct3d11_renderer SHARED ${SOURCE_FILES})
target_link_libraries		(direct3d11_renderer d3d11 d3dcompiler)
target_include_directories	(direct3d11_renderer PUBLIC include)
target_precompile_headers	(direct3d11_renderer PRIVATE src/pch.h)

install(TARGETS direct3d11_renderer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/renderers
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
