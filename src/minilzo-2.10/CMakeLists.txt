cmake_minimum_required(VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(MiniLZO_LIB_HEADER_DIR ${BorderlandsSaveEditor_INCLUDE_DIR}/minilzo-2.10)



set(MiniLZO_LIB_HEADER_FILES
        ${MiniLZO_LIB_HEADER_DIR}/lzoconf.h
        ${MiniLZO_LIB_HEADER_DIR}/lzodefs.h
        ${MiniLZO_LIB_HEADER_DIR}/minilzo.h
        ${CMAKE_CURRENT_BINARY_DIR}/minilzo_exports.h
        )

set(MiniLZO_LIB_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/minilzo.c
        )

add_library(EXTERNAL_MiniLZO_LIB SHARED
        ${MiniLZO_LIB_HEADER_FILES}
        ${MiniLZO_LIB_SOURCE_FILES}
        )

target_compile_options(EXTERNAL_MiniLZO_LIB
        PUBLIC -DEXTERNAL_MiniLZO_LIB_EXPORTS=1
        )

generate_export_header(EXTERNAL_MiniLZO_LIB
        BASE_NAME                   MiniLZO
        EXPORT_FILE_NAME            minilzo_exports.h
        EXPORT_MACRO_NAME           MINILZO_API
        DEPRECATED_MACRO_NAME       MINILZO_DEPRECATED
        NO_EXPORT_MACRO_NAME        MINILZO_NO_EXPORT
        NO_DEPRECATED_MACRO_NAME    MINILZO_API_NO_DEPRECATED
        )

set_target_properties(EXTERNAL_MiniLZO_LIB
        PROPERTIES
        OUTPUT_NAME     "MiniLZO"
        LANGUAGES       C
        VERSION         "2.10"
        )