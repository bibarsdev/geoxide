
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(geoxide-engine)

include(FindPythonInterp)

# for the time being, we use SDL as the only windowing system
set(GX_USE_SDL_WINDOW ON)

# Add third-party projects
add_subdirectory(thirdparty)

set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)
set(CGLTF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cgltf)

if(GX_USE_SDL_WINDOW)
	add_compile_definitions(GX_USE_SDL_WINDOW=1)
endif()

# Suppress warnings
if(MSVC)
	add_compile_options(/wd4267 /wd4172)
endif()

# Set platform
if(WIN32)
	add_compile_definitions(GX_PLATFORM_WIN32=1)
elseif(APPLE)
	add_compile_definitions(GX_PLATFORM_APPLE=1)
elseif(UNIX)
	add_compile_definitions(GX_PLATFORM_UNIX=1)
endif()

# Set compiler
if(MSVC)
	add_compile_definitions(GX_COMPILER_MSVC=1)
endif()

# Add sub-projects
add_subdirectory(geoxide_util)

include_directories(${geoxide_util_SOURCE_DIR}/include)
link_libraries(geoxide_util)

add_subdirectory(geoxide)
add_subdirectory(renderers)
add_subdirectory(converters)
add_subdirectory(test)
