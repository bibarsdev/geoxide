# imgui

cmake_minimum_required(VERSION 3.16)

project(imgui)

file(GLOB SOURCE_FILES *.cpp)

add_library				(imgui STATIC ${SOURCE_FILES})
target_include_directories		(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
	target_sources			(imgui PRIVATE backends/imgui_impl_dx11.cpp backends/imgui_impl_win32.cpp)
endif()

if(GX_USE_SDL_WINDOW)
	target_sources			(imgui PRIVATE backends/imgui_impl_sdl.cpp)
	target_include_directories	(imgui PRIVATE ${SDL2_SOURCE_DIR}/include)
	target_link_libraries		(imgui SDL2)
endif()

install(TARGETS imgui
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
