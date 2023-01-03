# converters

cmake_minimum_required(VERSION 3.16)

include_directories(include ${geoxide_SOURCE_DIR}/include)

function(add_converter target_name target_sources target_libs include_dirs target_definitions)
	
	add_executable				(${target_name} src/Main.cc ${${target_sources}})
	target_link_libraries		(${target_name} geoxide ${${target_libs}})
	target_precompile_headers	(${target_name} PRIVATE src/pch.h)
	target_include_directories	(${target_name} PUBLIC ${${include_dirs}})
	target_compile_definitions	(${target_name} PRIVATE ${${target_definitions}})
	
	if(WIN32)
		file(GLOB SOURCE_FILES src/Win32/*.cc)
		target_sources			(${target_name} PRIVATE ${${SOURCE_FILES}})
	endif()
	
	install(TARGETS ${target_name}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

endfunction()

set(MESH_SOURCES src/MeshConverter.cc src/TextureConverter.cc)
set(MESH_LIBS assimp)
set(MESH_INCLUDES ${Assimp_SOURCE_DIR}/include ${STB_INCLUDE_DIR})
set(MESH_DEFINES GX_MESH_CONVERTER=1)

set(TEXTURE_SOURCES src/TextureConverter.cc)
set(TEXTURE_LIBS geoxide)
set(TEXTURE_INCLUDES ${STB_INCLUDE_DIR})
set(TEXTURE_DEFINES GX_TEXTURE_CONVERTER=1)

add_converter(meshconv	MESH_SOURCES		MESH_LIBS		MESH_INCLUDES		MESH_DEFINES)
add_converter(texconv	TEXTURE_SOURCES		TEXTURE_LIBS	TEXTURE_INCLUDES	TEXTURE_DEFINES)
