# thirdparty

cmake_minimum_required(VERSION 3.16)

# SDL
if(GX_USE_SDL_WINDOW)
	set(SDL_TEST OFF)
	set(SDL_STATIC OFF)

	add_subdirectory(SDL2)
endif(GX_USE_SDL)

# shaderc
set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_EXAMPLES ON)

# add_subdirectory(shaderc)

