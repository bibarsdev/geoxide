# geoxide

cmake_minimum_required(VERSION 3.16)

project(geoxide_util)

file(GLOB HEADER_FILES include/Geoxide/*.h)
file(GLOB SOURCE_FILES src/*.cc)

add_library					(geoxide_util STATIC ${SOURCE_FILES})
target_include_directories	(geoxide_util PUBLIC include)
target_precompile_headers	(geoxide_util PUBLIC src/pch.h)

if(WIN32)
	file(GLOB SOURCE_FILES src/Win32/*.cc)
	target_sources		(geoxide_util PRIVATE ${SOURCE_FILES})
endif()

install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Geoxide)

install(TARGETS geoxide_util
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
