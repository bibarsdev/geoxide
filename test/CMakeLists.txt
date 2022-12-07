# test

cmake_minimum_required(VERSION 3.16)

file(GLOB SHADER_FILES assets/shaders/*.hlsl)
file(GLOB MODEL_FILES assets/models/*.model)
file(GLOB MATERIAL_FILES assets/materials/*.material)
file(GLOB TEXTURE_FILES assets/textures/*.texture)

file(GLOB SOURCE_FILES src/*.cc)

add_executable				(test ${SOURCE_FILES})
add_dependencies			(test direct3d11_renderer)
target_link_libraries		(test geoxide)
target_include_directories	(test PUBLIC include ${geoxide_SOURCE_DIR}/include)

install(FILES ${SHADER_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/shaders)
install(FILES ${MODEL_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/models)
install(FILES ${MATERIAL_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/materials)
install(FILES ${TEXTURE_FILES} DESTINATION ${CMAKE_INSTALL_BINDIR}/assets/textures)

install(TARGETS test
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
