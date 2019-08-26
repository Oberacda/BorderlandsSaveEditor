/**
 * @file        test_main.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Test main source file.
 * @version     0.1
 * @date        2019-08-26
 * 
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}