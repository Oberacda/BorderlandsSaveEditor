cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0087 NEW)

set(Borderlands3SaveEditor_LIB_PUBLIC_INCLUDE_FILES
		${CMAKE_CURRENT_BINARY_DIR}/save_editor_exports.hpp
		${Borderlands3SaveEditor_INCLUDE_DIR}/library.hpp
		${Borderlands3SaveEditor_INCLUDE_DIR}/savefiles.hpp
		)

set(Borderlands3SaveEditor_LIB_PRIVATE_INCLUDE_FILES)

set(Borderlands3SaveEditor_LIB_INCLUDE_FILES
		${Borderlands3SaveEditor_LIB_PUBLIC_INCLUDE_FILES}
		${Borderlands3SaveEditor_LIB_PRIVATE_INCLUDE_FILES}
		)

set(Borderlands3SaveEditor_LIB_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/library.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/savefile.cpp
		)

set(Borderlands3SaveEditor_LIB_RESOURCE_FILES)

if(WIN32)
	include(generate_product_version)
	generate_product_version(
		Borderlands3SaveEditor_LIB_VersionFilesOutputVariable
		NAME "SaveEditor"
		ICON "${Borderlands3SaveEditor_RESOURCE_DIR}/appicon.ico"
		VERSION_MAJOR 1
		VERSION_MINOR 0
		VERSION_PATH 0
		VERSION_REVISION ${BUILD_REVISION}
	)
	set(Borderlands3SaveEditor_LIB_RESOURCE_FILES
		${Borderlands3SaveEditor_LIB_RESOURCE_FILES}
		${Borderlands3SaveEditor_LIB_VersionFilesOutputVariable}
	)
endif(WIN32)

add_library(Borderlands3SaveEditor_LIB SHARED
		${Borderlands3SaveEditor_LIB_SOURCE_FILES}
		${Borderlands3SaveEditor_LIB_INCLUDE_FILES}
		${Borderlands3SaveEditor_LIB_RESOURCE_FILES}
		)

generate_export_header(Borderlands3SaveEditor_LIB
		BASE_NAME					Borderlands3SaveEditor
		EXPORT_FILE_NAME			save_editor_exports.hpp
		EXPORT_MACRO_NAME			PATHFINDER_SAVE_EDITOR_API
		DEPRECATED_MACRO_NAME		PATHFINDER_SAVE_EDITOR_API_DEPRECATED
		NO_EXPORT_MACRO_NAME		PATHFINDER_SAVE_EDITOR_API_NO_EXPORT
		NO_DEPRECATED_MACRO_NAME	PATHFINDER_SAVE_EDITOR_API_NO_DEPRECATED
		)

target_link_libraries(Borderlands3SaveEditor_LIB
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

target_compile_options(Borderlands3SaveEditor_LIB
		PUBLIC -DBorderlands3SaveEditor_LIB_EXPORTS=1
		)

set_target_properties(Borderlands3SaveEditor_LIB
		PROPERTIES
			OUTPUT_NAME			"SaveEditor"
			LANGUAGES			CXX
			PUBLIC_HEADERS		"${Borderlands3SaveEditor_LIB_PUBLIC_INCLUDE_FILES}"
			PRIVATE_HEADERS		"${Borderlands3SaveEditor_LIB_PRIVATE_INCLUDE_FILES}"
			VERSION				"${PROJECT_VERSION}"
		)


set(Borderlands3SaveEditor_EXE_INCLUDE_FILES
	${Borderlands3SaveEditor_INCLUDE_DIR}/main.h
	)

set(Borderlands3SaveEditor_EXE_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	)

set(Borderlands3SaveEditor_EXE_RESOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/Saveeditor_MainWindow.ui
		${Borderlands3SaveEditor_RESOURCE_DIR}/appicon.icns
)

if(WIN32)
	set(Borderlands3SaveEditor_EXE_SOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/main_windows.cpp
		${Borderlands3SaveEditor_EXE_SOURCE_FILES})

	include(generate_product_version)
	generate_product_version(
		Borderlands3SaveEditor_EXE_VersionFilesOutputVariable
		NAME ${PROJECT_NAME}
		ICON "${Borderlands3SaveEditor_RESOURCE_DIR}/appicon.ico"
		VERSION_MAJOR 1
		VERSION_MINOR 0
		VERSION_PATH 0
		VERSION_REVISION ${BUILD_REVISION}
	)
	set(Borderlands3SaveEditor_EXE_RESOURCE_FILES
		${Borderlands3SaveEditor_EXE_RESOURCE_FILES}
		${Borderlands3SaveEditor_EXE_VersionFilesOutputVariable}
	)
endif(WIN32)

add_executable(Borderlands3SaveEditor_EXE
	${Borderlands3SaveEditor_EXE_INCLUDE_FILES}
	${Borderlands3SaveEditor_EXE_SOURCE_FILES}
	${Borderlands3SaveEditor_EXE_RESOURCE_FILES}
	)


target_link_libraries(Borderlands3SaveEditor_EXE
		PUBLIC
			Borderlands3SaveEditor_LIB
			Qt5::Widgets
			Qt5::Gui
		)



set_target_properties(Borderlands3SaveEditor_EXE
		PROPERTIES
			OUTPUT_NAME			"Borderlands3SaveEditor"
			LANGUAGES			CXX
			VERSION				"${PROJECT_VERSION}"
			AUTOMOC				ON
			AUTOUIC				ON
		)

set(PLUGINS)

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

if(WIN32)
	set_target_properties(Borderlands3SaveEditor_EXE
		PROPERTIES
			WIN32_EXECUTABLE	ON
		)

	add_custom_command(TARGET Borderlands3SaveEditor_EXE POST_BUILD
			COMMAND "${CMAKE_COMMAND}" -E
			env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
			"$<TARGET_FILE:Borderlands3SaveEditor_EXE>"
			COMMENT "Running windeployqt..."
			)
elseif(APPLE)
	add_custom_command(TARGET Borderlands3SaveEditor_EXE POST_BUILD
			COMMAND "${MACDEPLOYQT_EXECUTABLE}"
			"$<TARGET_FILE_DIR:Borderlands3SaveEditor_EXE>/../.."
			-always-overwrite
			COMMENT "Running macdeployqt..."
			)

	set(MACOSX_BUNDLE_ICON_FILE appicon.icns)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.d4ve.games.Borderlands3SaveEditor")
	set(MACOSX_BUNDLE_LONG_VERSION_STRING ${Borderlands3SaveEditor_VERSION})
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${Borderlands3SaveEditor_VERSION})
	set(MACOSX_BUNDLE_BUNDLE_NAME Borderlands3SaveEditor)

	set_target_properties(Borderlands3SaveEditor_EXE PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${Borderlands3SaveEditor_RESOURCE_DIR}/MacOSXBundleInfo.plist.in)
	
	set_target_properties(Borderlands3SaveEditor_EXE
			PROPERTIES
			MACOSX_BUNDLE		ON
			RESOURCE "${Borderlands3SaveEditor_EXE_RESOURCE_FILES}"
			)

	get_target_property(QT_COCOA_LIBRARY Qt5::QCocoaIntegrationPlugin LOCATION)
	get_target_property(QT_MINIMAL_INTEGRATION_LIBRARY Qt5::QMinimalIntegrationPlugin LOCATION)

endif()

get_filename_component(ZLIB_LIBRARY_DIR ${ZLIB_LIBRARIES} DIRECTORY)


install(TARGETS Borderlands3SaveEditor_LIB
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

install(TARGETS Borderlands3SaveEditor_EXE
		RUNTIME
			DESTINATION bin
			COMPONENT Runtime
		BUNDLE
			DESTINATION .
			COMPONENT Runtime
		)