//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <common/common.hpp>

class StreamTest : public ::testing::Test {
protected:
    StreamTest() {
        // You can do set-up work for each test here.
    }

    ~StreamTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }
};

TEST_F(StreamTest, ReadUInt32_1) {
    SUCCEED();
}

TEST_F(StreamTest, ReadUInt32_2) {
    SUCCEED();
}

TEST_F(StreamTest, ReadUInt32_3) {
    SUCCEED();
}

TEST_F(StreamTest, ReadUInt32_4) {
    SUCCEED();
}