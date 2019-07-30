cmake_minimum_required(VERSION 3.11)

add_executable(TEST_MAIN_EXE
        ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/string-compare.cpp
        )

target_link_libraries(TEST_MAIN_EXE
        PUBLIC
        GTest::GTest
        BorderlandsSaveEditor_Borderlands2_LIB
        )

gtest_discover_tests(TEST_MAIN_EXE)