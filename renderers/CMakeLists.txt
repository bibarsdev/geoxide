# renderers

cmake_minimum_required(VERSION 3.16)

# Needed for implementation
include_directories(${geoxide_SOURCE_DIR}/include)

# Direct3D 11 Renderer
add_subdirectory(direct3d11)
