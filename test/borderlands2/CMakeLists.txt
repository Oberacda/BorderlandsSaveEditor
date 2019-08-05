cmake_minimum_required(VERSION 3.14)

set(BorderlandsSaveEditor_Borderlands2_LIB_TEST_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/borderlands2.cpp
        )

add_executable(BorderlandsSaveEditor_Borderlands2_LIB_TEST
        ${BorderlandsSaveEditor_Borderlands2_LIB_TEST_SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

target_link_libraries(BorderlandsSaveEditor_Borderlands2_LIB_TEST
        PUBLIC
        BorderlandsSaveEditor_Borderlands2_LIB
        PRIVATE
        GTest::GTest
        )

set_target_properties(BorderlandsSaveEditor_Borderlands2_LIB_TEST
        PROPERTIES
        OUTPUT_NAME     "Borderlands2SaveEditorTest"
        LANGUAGES       CXX
        VERSION         "${CMAKE_PROJECT_VERSION}"
        )

if(WIN32)
    get_filename_component(OPENSSL_LIBRARY_DIR "${OPENSSL_CRYPTO_LIBRARY}" DIRECTORY)

    install(CODE 
        "include(BundleUtilities)\n
        fixup_bundle(\"$<TARGET_FILE:BorderlandsSaveEditor_Borderlands2_LIB_TEST>\" \"\" \"${OPENSSL_LIBRARY_DIR}/../bin;${ZLIB_LIBRARY_DIR}/../bin;${LIBZIP_LIBRARY_DIR}/../bin;${Boost_LIBRARY_DIRS}\")"
        COMPONENT Runtime
        )

    install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DESTINATION .
        FILES_MATCHING PATTERN "*.dll")

endif(WIN32)

gtest_discover_tests(BorderlandsSaveEditor_Borderlands2_LIB_TEST)