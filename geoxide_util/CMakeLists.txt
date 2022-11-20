# geoxide

cmake_minimum_required(VERSION 3.16)

project(geoxide_util)

file(GLOB SOURCE_FILES src/*.cc)

add_library					(geoxide_util ${SOURCE_FILES})
target_include_directories	(geoxide_util PUBLIC include)
target_precompile_headers	(geoxide_util PUBLIC src/pch.h)

if(WIN32)
	file(GLOB SOURCE_FILES src/Win32/*.cc)
	target_sources		(geoxide_util PRIVATE ${SOURCE_FILES})
endif()
