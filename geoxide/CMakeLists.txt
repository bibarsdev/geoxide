# geoxide

cmake_minimum_required(VERSION 3.16)

project(geoxide)

file(GLOB SOURCE_FILES src/*.cc)

add_library					(geoxide ${SOURCE_FILES})
target_include_directories	(geoxide PUBLIC include ${SDL2_SOURCE_DIR}/include)
target_link_libraries		(geoxide direct3d11_renderer)
target_precompile_headers	(geoxide PUBLIC src/pch.h)

if(GX_USE_SDL_WINDOW)
	file(GLOB SOURCE_FILES src/SDL/*.cc)
	target_sources				(geoxide PRIVATE ${SOURCE_FILES})
	target_include_directories	(geoxide PRIVATE ${SDL2_SOURCE_DIR}/include)
	target_link_libraries		(geoxide SDL2)
endif()

if(WIN32)
	file(GLOB SOURCE_FILES src/Win32/*.cc)
	target_sources			(geoxide PRIVATE ${SOURCE_FILES})
endif()
