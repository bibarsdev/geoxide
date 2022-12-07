# meshconv

cmake_minimum_required(VERSION 3.16)

file(GLOB SOURCE_FILES src/*.cc)

add_executable				(meshconv ${SOURCE_FILES})
target_link_libraries		(meshconv geoxide assimp)
target_include_directories	(meshconv PUBLIC include ${geoxide_SOURCE_DIR}/include ${Assimp_SOURCE_DIR}/include)
target_precompile_headers	(meshconv PRIVATE src/pch.h)

if(WIN32)
	file(GLOB SOURCE_FILES src/Win32/*.cc)
	target_sources			(meshconv PRIVATE ${SOURCE_FILES})
endif()

install(TARGETS meshconv
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
