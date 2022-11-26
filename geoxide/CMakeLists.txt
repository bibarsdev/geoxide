# geoxide

cmake_minimum_required(VERSION 3.16)

project(geoxide)

file(GLOB HEADER_FILES include/Geoxide/*.h)
file(GLOB SOURCE_FILES src/*.cc)

add_library					(geoxide ${SOURCE_FILES})
target_include_directories	(geoxide PUBLIC include ${SDL2_SOURCE_DIR}/include)
target_precompile_headers	(geoxide PUBLIC src/pch.h)

if(GX_USE_SDL_WINDOW)
	file(GLOB SOURCE_FILES src/SDL/*.cc)
	target_sources				(geoxide PRIVATE ${SOURCE_FILES})
	target_include_directories	(geoxide PRIVATE ${SDL2_SOURCE_DIR}/include)
	target_link_libraries		(geoxide SDL2)
	install(TARGETS SDL2
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(WIN32)
	file(GLOB SOURCE_FILES src/Win32/*.cc)
	target_sources			(geoxide PRIVATE ${SOURCE_FILES})
endif()

install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Geoxide)

install(TARGETS geoxide
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
