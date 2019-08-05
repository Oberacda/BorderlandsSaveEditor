cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0087 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/minilzo-2.10)

add_subdirectory(minilzo-2.10)
add_subdirectory(common)
add_subdirectory(borderlands2)
add_subdirectory(save_editor)
