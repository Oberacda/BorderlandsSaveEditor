cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0087 NEW)

set(BorderlandsSaveEditor_LIB_PUBLIC_INCLUDE_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/bl_save_editor_exports.hpp
#        ${BorderlandsSaveEditor_INCLUDE_DIR}/library.hpp
        ${BorderlandsSaveEditor_INCLUDE_DIR}/savefiles.hpp
        )

set(BorderlandsSaveEditor_LIB_PRIVATE_INCLUDE_FILES)

set(BorderlandsSaveEditor_LIB_INCLUDE_FILES
        ${BorderlandsSaveEditor_LIB_PUBLIC_INCLUDE_FILES}
        ${BorderlandsSaveEditor_LIB_PRIVATE_INCLUDE_FILES}
        )

set(BorderlandsSaveEditor_LIB_SOURCE_FILES
#        ${CMAKE_CURRENT_SOURCE_DIR}/library.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/savefile.cpp
        )

set(BorderlandsSaveEditor_LIB_RESOURCE_FILES)

if (WIN32)
    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_LIB_VersionFilesOutputVariable
            NAME                "SaveEditor"
            ICON                "${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.ico"
            VERSION_MAJOR       ${CMAKE_PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${CMAKE_PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${CMAKE_PROJECT_VERSION_TWEAK}
            VERSION_REVISION    ${CMAKE_PROJECT_VERSION_PATCH}
    )
    set(BorderlandsSaveEditor_LIB_RESOURCE_FILES
            ${BorderlandsSaveEditor_LIB_RESOURCE_FILES}
            ${BorderlandsSaveEditor_LIB_VersionFilesOutputVariable}
            )
endif (WIN32)

add_library(BorderlandsSaveEditor_LIB SHARED
        ${BorderlandsSaveEditor_LIB_SOURCE_FILES}
        ${BorderlandsSaveEditor_LIB_INCLUDE_FILES}
        ${BorderlandsSaveEditor_LIB_RESOURCE_FILES}
        )

generate_export_header(BorderlandsSaveEditor_LIB
        BASE_NAME                   BorderlandsSaveEditor
        EXPORT_FILE_NAME            bl_save_editor_exports.hpp
        EXPORT_MACRO_NAME           BORDERLANDS_SAVE_EDITOR_API
        DEPRECATED_MACRO_NAME       BORDERLANDS_SAVE_EDITOR_API_DEPRECATED
        NO_EXPORT_MACRO_NAME        BORDERLANDS_SAVE_EDITOR_API_NO_EXPORT
        NO_DEPRECATED_MACRO_NAME    BORDERLANDS_SAVE_EDITOR_API_NO_DEPRECATED
        )

target_link_libraries(BorderlandsSaveEditor_LIB
        PUBLIC
        Boost::atomic
        Boost::chrono
        Boost::date_time
        Boost::filesystem
        Boost::log
        Boost::log_setup
        Boost::system
        Boost::thread
        Boost::regex
        ${LIBZIP_LIBRARY}
        )

target_compile_options(BorderlandsSaveEditor_LIB
        PUBLIC -DBorderlandsSaveEditor_LIB_EXPORTS=1
        )

set_target_properties(BorderlandsSaveEditor_LIB
        PROPERTIES
        OUTPUT_NAME     "SaveEditor"
        LANGUAGES       CXX
        PUBLIC_HEADERS  "${BorderlandsSaveEditor_LIB_PUBLIC_INCLUDE_FILES}"
        PRIVATE_HEADERS "${BorderlandsSaveEditor_LIB_PRIVATE_INCLUDE_FILES}"
        VERSION         "${PROJECT_VERSION}"
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_PUBLIC_INCLUDE_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/bl2_save_editor_exports.hpp
        ${BorderlandsSaveEditor_INCLUDE_DIR}/library.hpp
        #${BorderlandsSaveEditor_INCLUDE_DIR}/savefiles.hpp
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_PRIVATE_INCLUDE_FILES)

set(BorderlandsSaveEditor_Borderlands2_LIB_INCLUDE_FILES
        ${BorderlandsSaveEditor_LIB_PUBLIC_INCLUDE_FILES}
        ${BorderlandsSaveEditor_LIB_PRIVATE_INCLUDE_FILES}
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/library.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/savefile.cpp
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES)

if (WIN32)
    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_Borderlands2_VersionFilesOutputVariable
            NAME                "Borderlands2SaveEditor"
            ICON                "${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.ico"
            VERSION_MAJOR       ${CMAKE_PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${CMAKE_PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${CMAKE_PROJECT_VERSION_PATCH}
            VERSION_REVISION    ${CMAKE_PROJECT_VERSION_TWEAK}
    )
    set(BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES
            ${BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES}
            ${BorderlandsSaveEditor_Borderlands2_LIB_VersionFilesOutputVariable}
            )
endif (WIN32)

add_library(BorderlandsSaveEditor_Borderlands2_LIB SHARED
        ${BorderlandsSaveEditor_Borderlands2_LIB_SOURCE_FILES}
        ${BorderlandsSaveEditor_Borderlands2_LIB_INCLUDE_FILES}
        ${BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES}
        )

generate_export_header(BorderlandsSaveEditor_Borderlands2_LIB
        BASE_NAME                   Borderlands2SaveEditor
        EXPORT_FILE_NAME            bl2_save_editor_exports.hpp
        EXPORT_MACRO_NAME           BORDERLANDS2_SAVE_EDITOR_API
        DEPRECATED_MACRO_NAME       BORDERLANDS2_SAVE_EDITOR_API_DEPRECATED
        NO_EXPORT_MACRO_NAME        BORDERLANDS2_SAVE_EDITOR_API_NO_EXPORT
        NO_DEPRECATED_MACRO_NAME    BORDERLANDS2_SAVE_EDITOR_API_NO_DEPRECATED
        )

target_link_libraries(BorderlandsSaveEditor_Borderlands2_LIB
        PUBLIC
        BorderlandsSaveEditor_LIB
        )

target_compile_options(BorderlandsSaveEditor_Borderlands2_LIB
        PUBLIC -DBorderlands2SaveEditor_LIB_EXPORTS=1
        )

set_target_properties(BorderlandsSaveEditor_Borderlands2_LIB
        PROPERTIES
        OUTPUT_NAME     "Borderlands2SaveEditor"
        LANGUAGES       CXX
        PUBLIC_HEADERS  "${BorderlandsSaveEditor_Borderlands2_LIB_PUBLIC_INCLUDE_FILES}"
        PRIVATE_HEADERS "${BorderlandsSaveEditor_Borderlands2_LIB_PRIVATE_INCLUDE_FILES}"
        VERSION         "${CMAKE_PROJECT_VERSION}"
        )


set(BorderlandsSaveEditor_EXE_INCLUDE_FILES
        ${BorderlandsSaveEditor_INCLUDE_DIR}/main.h
        )

set(BorderlandsSaveEditor_EXE_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

set(BorderlandsSaveEditor_EXE_RESOURCE_FILES

        ${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.icns
        )

set(BorderlandsSaveEditor_EXE_PRIVATE_RESOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Saveeditor_MainWindow.ui
        )

if (WIN32)
    set(BorderlandsSaveEditor_EXE_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/main_windows.cpp
            ${BorderlandsSaveEditor_EXE_SOURCE_FILES})

    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_EXE_VersionFilesOutputVariable
            NAME                ${PROJECT_NAME}
            ICON                "${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.ico"
            VERSION_MAJOR       ${CMAKE_PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${CMAKE_PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${CMAKE_PROJECT_VERSION_PATCH}
            VERSION_REVISION    ${CMAKE_PROJECT_VERSION_TWEAK}
    )
    set(BorderlandsSaveEditor_EXE_RESOURCE_FILES
            ${BorderlandsSaveEditor_EXE_RESOURCE_FILES}
            ${BorderlandsSaveEditor_EXE_VersionFilesOutputVariable}
            )
endif (WIN32)

add_executable(BorderlandsSaveEditor_EXE
        ${BorderlandsSaveEditor_EXE_INCLUDE_FILES}
        ${BorderlandsSaveEditor_EXE_SOURCE_FILES}
        ${BorderlandsSaveEditor_EXE_RESOURCE_FILES}
        ${BorderlandsSaveEditor_EXE_PRIVATE_RESOURCE_FILES}
        )


target_link_libraries(BorderlandsSaveEditor_EXE
        PUBLIC
        BorderlandsSaveEditor_LIB
        BorderlandsSaveEditor_Borderlands2_LIB
        Qt5::Widgets
        Qt5::Gui
        )


set_target_properties(BorderlandsSaveEditor_EXE
        PROPERTIES
        OUTPUT_NAME     "BorderlandsSaveEditor"
        LANGUAGES       CXX
        VERSION         "${CMAKE_PROJECT_VERSION}"
        AUTOMOC         ON
        AUTOUIC         ON
        )

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

if (WIN32)
    set_target_properties(BorderlandsSaveEditor_EXE
            PROPERTIES
            WIN32_EXECUTABLE ON
            )

    add_custom_command(TARGET BorderlandsSaveEditor_EXE POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE:BorderlandsSaveEditor_EXE>"
            COMMENT "Running windeployqt..."
            )
elseif (APPLE)
    add_custom_command(TARGET BorderlandsSaveEditor_EXE POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE_DIR:BorderlandsSaveEditor_EXE>/../.."
            -always-overwrite
            COMMENT "Running macdeployqt..."
            )

    set(MACOSX_BUNDLE_ICON_FILE             appicon.icns)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER        "com.d4ve.games.BorderlandsSaveEditor")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING   ${BorderlandsSaveEditor_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING  ${BorderlandsSaveEditor_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_NAME           BorderlandsSaveEditor)

    set_target_properties(BorderlandsSaveEditor_EXE
            PROPERTIES
            MACOSX_BUNDLE               ON
            MACOSX_BUNDLE_INFO_PLIST    ${BorderlandsSaveEditor_RESOURCE_DIR}/MacOSXBundleInfo.plist.in
            RESOURCE                    "${BorderlandsSaveEditor_EXE_RESOURCE_FILES}"
            )

endif ()

get_filename_component(ZLIB_LIBRARY_DIR ${ZLIB_LIBRARIES} DIRECTORY)


install(TARGETS BorderlandsSaveEditor_LIB
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

install(TARGETS BorderlandsSaveEditor_EXE
        RUNTIME
        DESTINATION bin
        COMPONENT Runtime
        BUNDLE
        DESTINATION .
        COMPONENT Runtime
        )

if(WIN32)
    install(CODE 
        "include(BundleUtilities)\n
        fixup_bundle(\"$<TARGET_FILE:BorderlandsSaveEditor_EXE>\" \"\" \"${ZLIB_LIBRARY_DIR}/../bin;${LIBZIP_LIBRARY_DIR}/../bin;${Boost_LIBRARY_DIRS}\")"
        COMPONENT Runtime
        )

    install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DESTINATION .
        FILES_MATCHING PATTERN "*.dll")

endif(WIN32)

