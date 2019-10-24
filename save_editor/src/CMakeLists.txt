cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0087 NEW)

set(Borderlands_SaveEditor_EXE_INCLUDE_FILES
        ${Borderlands_SaveEditor_EXE_INCLUDE_DIR}/main.hpp
        )

set(Borderlands_SaveEditor_EXE_SOURCE_FILES
        )

set(Borderlands_SaveEditor_EXE_RESOURCE_FILES
        ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/appicon.icns
        )

# Using a source directory:
set(Borderlands_SaveEditor_EXE_TRANSLATION_FILE
        ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/langs/save_editor_de.ts
        ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/langs/save_editor_en.ts
        )

qt5_create_translation(QM_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Borderlands_SaveEditor_EXE_TRANSLATION_FILE}
        )

configure_file(
        ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/langs/translations.qrc
        ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
        COPYONLY
)

set(Borderlands_SaveEditor_EXE_PRIVATE_RESOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/SaveEditor_MainWindow.ui
        ${QM_FILES}
        ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
        ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/image_resources.qrc
        )

if (WIN32)
    set(Borderlands_SaveEditor_EXE_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/main_windows.cpp
            ${Borderlands_SaveEditor_EXE_SOURCE_FILES})

    include(generate_product_version)
    generate_product_version(
            Borderlands_SaveEditor_EXE_VersionFilesOutputVariable
            NAME                ${PROJECT_NAME}
            ICON                ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/appicon.ico
            VERSION_MAJOR       ${CMAKE_PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${CMAKE_PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${CMAKE_PROJECT_VERSION_PATCH}
            VERSION_REVISION    ${CMAKE_PROJECT_VERSION_TWEAK}
    )
    set(Borderlands_SaveEditor_EXE_RESOURCE_FILES
            ${Borderlands_SaveEditor_EXE_RESOURCE_FILES}
            ${Borderlands_SaveEditor_EXE_VersionFilesOutputVariable}
            )
endif (WIN32)



add_executable(Borderlands_SaveEditor_EXE
        ${Borderlands_SaveEditor_EXE_INCLUDE_FILES}
        ${Borderlands_SaveEditor_EXE_SOURCE_FILES}
        ${Borderlands_SaveEditor_EXE_RESOURCE_FILES}
        ${Borderlands_SaveEditor_EXE_PRIVATE_RESOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )


target_link_libraries(Borderlands_SaveEditor_EXE
        PUBLIC
        Borderlands2_Save_LIB
        Qt5::Widgets
        Qt5::Gui
        )


set_target_properties(Borderlands_SaveEditor_EXE
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
    set_target_properties(Borderlands_SaveEditor_EXE
            PROPERTIES
            WIN32_EXECUTABLE ON
            )

    add_custom_command(TARGET Borderlands_SaveEditor_EXE POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE:Borderlands_SaveEditor_EXE>"
            COMMENT "Running windeployqt..."
            )
elseif (APPLE)
    add_custom_command(TARGET Borderlands_SaveEditor_EXE POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE_DIR:Borderlands_SaveEditor_EXE>/../.."
            -always-overwrite
            COMMENT "Running macdeployqt..."
            )

    set(MACOSX_BUNDLE_ICON_FILE             appicon.icns)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER        "com.d4ve.games.BorderlandsSaveEditor")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING   ${BorderlandsSaveEditor_VERSION})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING  ${BorderlandsSaveEditor_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_NAME           BorderlandsSaveEditor)

    set_target_properties(Borderlands_SaveEditor_EXE
            PROPERTIES
            MACOSX_BUNDLE               ON
            MACOSX_BUNDLE_INFO_PLIST    ${Borderlands_SaveEditor_EXE_RESOURCE_DIR}/MacOSXBundleInfo.plist.in
            RESOURCE                    "${Borderlands_SaveEditor_EXE_RESOURCE_FILES}"
            )

endif ()

get_filename_component(ZLIB_LIBRARY_DIR "${ZLIB_LIBRARIES}" DIRECTORY)
get_filename_component(OPENSSL_LIBRARY_DIR "${OPENSSL_CRYPTO_LIBRARY}" DIRECTORY)

install(TARGETS Borderlands_SaveEditor_EXE
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
        fixup_bundle(\"$<TARGET_FILE:Borderlands_SaveEditor_EXE>\" \"\" \"${OPENSSL_LIBRARY_DIR}/../bin;${ZLIB_LIBRARY_DIR}/../bin;${LIBZIP_LIBRARY_DIR}/../bin;${Boost_LIBRARY_DIRS}\")"
            COMPONENT Runtime
            )

    install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            DESTINATION .
            FILES_MATCHING PATTERN "*.dll")

endif(WIN32)