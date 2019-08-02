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
        GTest::GTest
        BorderlandsSaveEditor_Borderlands2_LIB
        )

set_target_properties(BorderlandsSaveEditor_Borderlands2_LIB_TEST
        PROPERTIES
        OUTPUT_NAME     "Borderlands2SaveEditorTest"
        LANGUAGES       CXX
        VERSION         "${CMAKE_PROJECT_VERSION}"
        )

gtest_discover_tests(BorderlandsSaveEditor_Borderlands2_LIB_TEST)