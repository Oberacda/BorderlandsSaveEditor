cmake_minimum_required(VERSION 3.14)

set(Borderlands_Common_LIB_TEST_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
        )

add_executable(Borderlands_Common_LIB_TEST
        ${Borderlands_Common_LIB_TEST_SOURCE_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

target_link_libraries(Borderlands_Common_LIB_TEST
        PUBLIC
        GTest::GTest
        Borderlands_Common_LIB
        )

set_target_properties(Borderlands_Common_LIB_TEST
        PROPERTIES
        OUTPUT_NAME     "BorderlandsCommonTest"
        LANGUAGES       CXX
        VERSION         "${CMAKE_PROJECT_VERSION}"
        )

gtest_discover_tests(Borderlands_Common_LIB_TEST)