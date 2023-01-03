# geoxide

cmake_minimum_required(VERSION 3.16)

project(geoxide)

file(GLOB LIT_SHADER_FILES assets/shaders/Lit/*.hlsl)
file(GLOB UNLIT_SHADER_FILES assets/shaders/Unlit/*.hlsl)
file(GLOB LIT_ISHADER_FILES assets/shaders/Lit/*.hlsli)
file(GLOB UNLIT_ISHADER_FILES assets/shaders/Unlit/*.hlsli)
file(GLOB MODEL_FILES assets/models/*.model)
file(GLOB MATERIAL_FILES assets/materials/*.material)
file(GLOB TEXTURE_FILES assets/textures/*.texture)

file(GLOB HEADER_FILES include/Geoxide/*.h)
file(GLOB SOURCE_FILES src/*.cc)

add_library					(geoxide STATIC ${SOURCE_FILES})
target_link_libraries		(geoxide imgui)
target_include_directories	(geoxide PUBLIC include ${imgui_SOURCE_DIR})
target_precompile_headers	(geoxide PRIVATE src/pch.h)

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

install(FILES ${LIT_SHADER_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/shaders/Lit)
install(FILES ${UNLIT_SHADER_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/shaders/Unlit)
install(FILES ${LIT_ISHADER_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/shaders/Lit)
install(FILES ${UNLIT_ISHADER_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/shaders/Unlit)
install(FILES ${MODEL_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/models)
install(FILES ${MATERIAL_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/materials)
install(FILES ${TEXTURE_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/textures)

install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Geoxide)

install(TARGETS geoxide
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
