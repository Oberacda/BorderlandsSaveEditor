/**
 * @file        test_common.cpp
 * @author      David Oberacker (david.oberacker@gmail.com)
 * @brief       Test source file for the common.hpp functions.
 * @version     0.1
 * @date        2019-08-26
 * 
 * @copyright   Copyright (c) 2019 David Oberacker
 */

#include <gtest/gtest.h>
#include <common/common.hpp>

/**
 * @brief Class to test the D4v3::Borderlands::Common::Streams functions.
 */
class StreamTest : public ::testing::Test {
protected:
    /**
     * @brief Construct a new Stream Test object
     */
    StreamTest() {
        // You can do set-up work for each test here.
    }

    /**
     * @brief Destroy the Stream Test object
     */
    ~StreamTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    /**
     * @brief Set up the test env before every test.
     */
    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    /**
     * @brief Tear down the test env after every test.
     */
    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

/**
 * @brief Test to check read functions.
 * @test Tests the readUInt32 function.
 */
TEST_F(StreamTest, ReadUInt32_1) {
    SUCCEED();
}

/**
 * @brief Test to check read functions.
 * @test Tests the readUInt32 function.
 */
TEST_F(StreamTest, ReadUInt32_2) {
    SUCCEED();
}

/**
 * @brief Test to check read functions.
 * @test Tests the readUInt32 function.
 */
TEST_F(StreamTest, ReadUInt32_3) {
    SUCCEED();
}

/**
 * @brief Test to check read functions.
 * @test Tests the readUInt32 function.
 */
TEST_F(StreamTest, ReadUInt32_4) {
    SUCCEED();
}