cmake_minimum_required(VERSION 3.14)

set(Borderlands_Common_LIB_PUBLIC_INCLUDE_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/bl_common_exports.hpp
        ${Borderlands_Common_LIB_INCLUDE_DIR}/common/common.hpp
        )

set(Borderlands_Common_LIB_PRIVATE_INCLUDE_FILES)

set(Borderlands_Common_LIB_INCLUDE_FILES
        ${Borderlands_Common_LIB_PUBLIC_INCLUDE_FILES}
        ${Borderlands_Common_LIB_PRIVATE_INCLUDE_FILES}
        )

set(Borderlands_Common_LIB_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/decoder.cpp
        )

set(Borderlands_Common_LIB_RESOURCE_FILES)

if (WIN32)
    include(generate_product_version)
    generate_product_version(
            Borderlands_Common_LIB_VersionFilesOutputVariable
            NAME                "Borderlands_Common"
            ICON                ${Borderlands_Common_LIB_RESOURCE_DIR}/appicon.ico
            VERSION_MAJOR       ${PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${PROJECT_VERSION_TWEAK}
            VERSION_REVISION    ${PROJECT_VERSION_PATCH}
    )
    set(Borderlands_Common_LIB_RESOURCE_FILES
            ${Borderlands_Common_LIB_RESOURCE_FILES}
            ${Borderlands_Common_LIB_VersionFilesOutputVariable}
            )
endif (WIN32)

add_library(Borderlands_Common_LIB STATIC
        ${Borderlands_Common_LIB_SOURCE_FILES}
        ${Borderlands_Common_LIB_INCLUDE_FILES}
        ${Borderlands_Common_LIB_RESOURCE_FILES}
        )

generate_export_header(Borderlands_Common_LIB
        BASE_NAME                   BorderlandsCommon
        EXPORT_FILE_NAME            bl_common_exports.hpp
        EXPORT_MACRO_NAME           BORDERLANDS_COMMON_API
        DEPRECATED_MACRO_NAME       BORDERLANDS_COMMON_API_DEPRECATED
        NO_EXPORT_MACRO_NAME        BORDERLANDS_COMMON_API_NO_EXPORT
        NO_DEPRECATED_MACRO_NAME    BORDERLANDS_COMMON_API_NO_DEPRECATED
        )

target_include_directories(Borderlands_Common_LIB
        PUBLIC ${Borderlands_Common_LIB_INCLUDE_DIR}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(Borderlands_Common_LIB
        PUBLIC
        Boost::boost
        )

target_compile_options(Borderlands_Common_LIB
        PRIVATE -DBorderlands_Common_LIB_EXPORTS=1
        PUBLIC  -fPIC
        )

set_target_properties(Borderlands_Common_LIB
        PROPERTIES
        OUTPUT_NAME     "BorderlandsCommon"
        LANGUAGES       CXX
        LINKER_LANGUAGE CXX
        PUBLIC_HEADERS  "${Borderlands_Common_LIB_PUBLIC_INCLUDE_FILES}"
        PRIVATE_HEADERS "${Borderlands_Common_LIB_PRIVATE_INCLUDE_FILES}"
        VERSION         "${PROJECT_VERSION}"
        )

install(TARGETS Borderlands_Common_LIB
        CONFIGURATIONS Debug RelWithDebInfo
        ARCHIVE
        DESTINATION lib
        COMPONENT Development
        LIBRARY
        DESTINATION lib
        COMPONENT Libraries
        NAMELINK_COMPONENT Development
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT Development
        )
