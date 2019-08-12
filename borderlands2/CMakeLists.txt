cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0087 NEW)

set(Borderlands2_Save_LIB_VERSION "1.0.0.1")

project(Borderlands2SaveLib
        LANGUAGES   C CXX
        VERSION     ${Borderlands2_Save_LIB_VERSION}
        DESCRIPTION "Save editing library for Borderlands 2"
        )

find_package(Boost 1.67 REQUIRED
        COMPONENTS
        atomic
        chrono
        date_time
        filesystem
        log
        log_setup
        system
        thread
        regex
        )
find_package(OpenSSL COMPONENTS Crypto REQUIRED)
find_package(GTest)
find_package(Doxygen)
find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

set(Borderlands2_Save_LIB_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(Borderlands2_Save_LIB_RESOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        )

add_subdirectory(src)

option(Borderlands2_Save_LIB_TESTING_ENABLED "Enabled testing of the project!" ON)

if (GTEST_FOUND AND Borderlands2_Save_LIB_TESTING_ENABLED)
    enable_testing()
    add_subdirectory(test)       

    message(STATUS "Testing enabled!")
endif (GTEST_FOUND AND Borderlands2_Save_LIB_TESTING_ENABLED)

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY   ${PROJECT_DESCRIPTION})
SET(CPACK_PACKAGE_VENDOR                "D4ve")
SET(CPACK_PACKAGE_DESCRIPTION_FILE      "${CMAKE_CURRENT_SOURCE_DIR}/Readme.txt")
SET(CPACK_RESOURCE_FILE_LICENSE         "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR         ${PROJECT_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR         ${PROJECT_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH         ${PROJECT_VERSION_PATCH})

IF (WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    SET(CPACK_PACKAGE_ICON              "${CMAKE_CURRENT_SOURCE_DIR}/resources\\\\appicon.ico")
    SET(CPACK_NSIS_INSTALLED_ICON_NAME  "bin\\\\BorderlandsSaveEditor.exe")
    SET(CPACK_NSIS_DISPLAY_NAME         ${PROJECT_NAME})
    SET(CPACK_NSIS_HELP_LINK            "http:\\\\\\\\www.d4ve.org")
    SET(CPACK_NSIS_URL_INFO_ABOUT       "http:\\\\\\\\www.d4ve.com")
    SET(CPACK_NSIS_CONTACT              "david.oberacker@student.kit.edu")
    SET(CPACK_NSIS_MODIFY_PATH          ON)
ELSE (WIN32 AND NOT UNIX)
    SET(CPACK_STRIP_FILES               "bin/BorderlandsSaveEditor")
    SET(CPACK_SOURCE_STRIP_FILES        "")
ENDIF (WIN32 AND NOT UNIX)

INCLUDE(CPack)

        