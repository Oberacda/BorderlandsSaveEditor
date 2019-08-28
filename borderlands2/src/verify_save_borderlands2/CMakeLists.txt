cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0087 NEW)

set(Borderlands2_VerifySave_EXE_HEADER_DIR ${Borderlands2_INCLUDE_DIR}/verify_save)

set(Borderlands2_VerifySave_EXE_PUBLIC_INCLUDE_FILES)

set(Borderlands2_VerifySave_EXE_PRIVATE_INCLUDE_FILES)

set(Borderlands2_VerifySave_EXE_INCLUDE_FILES
        ${Borderlands2_VerifySave_EXE_PUBLIC_INCLUDE_FILES}
        ${Borderlands2_VerifySave_EXE_PRIVATE_INCLUDE_FILES}
        )

set(Borderlands2_VerifySave_EXE_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

set(Borderlands2_VerifySave_EXE_RESOURCE_FILES)

if (WIN32)
    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_Borderlands2_VersionFilesOutputVariable
            NAME                "Borderlands2_VerifySave"
            VERSION_MAJOR       ${PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${PROJECT_VERSION_PATCH}
            VERSION_REVISION    ${PROJECT_VERSION_TWEAK}
    )
    set(Borderlands2_VerifySave_EXE_RESOURCE_FILES
            ${Borderlands2_VerifySave_EXE_RESOURCE_FILES}
            ${Borderlands2_VerifySave_EXE_VersionFilesOutputVariable}
            )
endif (WIN32)

add_executable(Borderlands2_VerifySave_EXE
        ${Borderlands2_VerifySave_EXE_SOURCE_FILES}
        ${Borderlands2_VerifySave_EXE_INCLUDE_FILES}
        ${Borderlands2_VerifySave_EXE_RESOURCE_FILES}
        )

target_include_directories(Borderlands2_VerifySave_EXE
        PUBLIC ${Borderlands2_VerifySave_EXE_HEADER_DIR}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Borderlands2_VerifySave_EXE
        PUBLIC
        Borderlands2_Save_LIB
        PRIVATE
        cxxopts::cxxopts
        )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(Borderlands2_VerifySave_EXE
            PRIVATE
            c++fs
            )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(Borderlands2_VerifySave_EXE
            PRIVATE
            stdc++fs
            )
endif()

target_compile_options(Borderlands2_VerifySave_EXE
        PRIVATE -DBorderlands2_VerifySave_EXE_EXPORTS=1
        )

set_target_properties(Borderlands2_VerifySave_EXE
        PROPERTIES
        OUTPUT_NAME     "Borderlands2SaveVerify"
        LANGUAGES       CXX
        PUBLIC_HEADERS  "${Borderlands2_VerifySave_EXE_PUBLIC_INCLUDE_FILES}"
        PRIVATE_HEADERS "${Borderlands2_VerifySave_EXE_PRIVATE_INCLUDE_FILES}"
        VERSION         "${PROJECT_VERSION}"
        CXX_STANDARD    17
        )

install(TARGETS Borderlands2_VerifySave_EXE
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
