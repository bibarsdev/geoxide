# thirdparty

cmake_minimum_required(VERSION 3.16)

# SDL
if(GX_USE_SDL_WINDOW)
	set(SDL_TEST OFF)
	set(SDL_STATIC OFF)

	add_subdirectory(SDL2)
endif()
