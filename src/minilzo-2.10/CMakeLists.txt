cmake_minimum_required(VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(MiniLZO_LIB_HEADER_DIR ${BorderlandsSaveEditor_INCLUDE_DIR}/minilzo-2.10)

set(MiniLZO_LIB_HEADER_FILES
        ${MiniLZO_LIB_HEADER_DIR}/lzoconf.h
        ${MiniLZO_LIB_HEADER_DIR}/lzodefs.h
        ${MiniLZO_LIB_HEADER_DIR}/minilzo.h
        )

set(MiniLZO_LIB_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/minilzo.c
        )

add_library(EXTERNAL_MiniLZO_LIB SHARED
        ${MiniLZO_LIB_HEADER_FILES}
        ${MiniLZO_LIB_SOURCE_FILES}
        )

set_target_properties(EXTERNAL_MiniLZO_LIB
        PROPERTIES
        OUTPUT_NAME     "MiniLZO"
        LANGUAGES       C
        VERSION         "2.10"
        )