cmake_minimum_required(VERSION 3.14)

set(Borderlands2_Save_LIB_TEST_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/test_borderlands2.cpp
        )

add_executable(Borderlands2_Save_LIB_TEST
        ${Borderlands2_Save_LIB_TEST_SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
        )

target_link_libraries(Borderlands2_Save_LIB_TEST
        PUBLIC
        Borderlands2_Save_LIB
        PRIVATE
        GTest::GTest
        )

set_target_properties(Borderlands2_Save_LIB_TEST
        PROPERTIES
        OUTPUT_NAME     "Borderlands2SaveLibTest"
        LANGUAGES       CXX
        VERSION         "${CMAKE_PROJECT_VERSION}"
        CXX_STANDARD    17
        )

if(WIN32)
    get_filename_component(OPENSSL_LIBRARY_DIR "${OPENSSL_CRYPTO_LIBRARY}" DIRECTORY)

    install(CODE 
        "include(BundleUtilities)\n
        fixup_bundle(\"$<TARGET_FILE:Borderlands2_Save_LIB_TEST>\" \"\" \"${OPENSSL_LIBRARY_DIR}/../bin;${ZLIB_LIBRARY_DIR}/../bin;${LIBZIP_LIBRARY_DIR}/../bin;${Boost_LIBRARY_DIRS}\")"
        COMPONENT Runtime
        )

    install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DESTINATION .
        FILES_MATCHING PATTERN "*.dll")

endif(WIN32)

gtest_discover_tests(Borderlands2_Save_LIB_TEST)