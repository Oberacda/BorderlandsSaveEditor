cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0074 NEW)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
        )

include(GenerateExportHeader)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)

set(BorderlandsSaveEditor_VERSION "1.0.0.1")

project(BorderlandsSaveEditor
        LANGUAGES    CXX
        VERSION     ${BorderlandsSaveEditor_VERSION}
        DESCRIPTION "Save editor for Borderlands 3"
        )
enable_testing()

add_subdirectory(common)
add_subdirectory(borderlands2)
