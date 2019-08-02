cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0087 NEW)

set(BorderlandsSaveEditor_EXE_INCLUDE_DIR ${BorderlandsSaveEditor_INCLUDE_DIR}/save_editor)

set(BorderlandsSaveEditor_EXE_INCLUDE_FILES
        ${BorderlandsSaveEditor_EXE_INCLUDE_DIR}/main.h
        )

set(BorderlandsSaveEditor_EXE_SOURCE_FILES
        )

set(BorderlandsSaveEditor_EXE_RESOURCE_FILES
        ${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.icns
        )

# Using a source directory:
file(GLOB TS_FILES "${BorderlandsSaveEditor_RESOURCE_DIR}/langs/*.ts")

qt5_create_translation(QM_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${TS_FILES}
        )

configure_file(
        ${BorderlandsSaveEditor_RESOURCE_DIR}/langs/translations.qrc
        ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
        COPYONLY
)

set(BorderlandsSaveEditor_EXE_PRIVATE_RESOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/SaveEditor_MainWindow.ui
        ${QM_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
        ${BorderlandsSaveEditor_RESOURCE_DIR}/image_resources.qrc
        )

if (WIN32)
    set(BorderlandsSaveEditor_EXE_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/main_windows.cpp
            ${BorderlandsSaveEditor_EXE_SOURCE_FILES})

    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_EXE_VersionFilesOutputVariable
            NAME                ${PROJECT_NAME}
            ICON                ${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.ico
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
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )


target_link_libraries(BorderlandsSaveEditor_EXE
        PUBLIC
        Borderlands_Common_LIB
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
        AUTORCC         ON
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
