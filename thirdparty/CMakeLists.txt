# thirdparty

cmake_minimum_required(VERSION 3.16)

# SDL
if(GX_USE_SDL_WINDOW)
	set(SDL_TEST OFF)
	set(SDL_STATIC OFF)
	set(SDL2_DISABLE_INSTALL OFF)
	
	add_subdirectory(SDL2)
endif()

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(BUILD_SHARED_LIBS ON)

add_subdirectory(assimp)
