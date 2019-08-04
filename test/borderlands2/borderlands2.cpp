//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <borderlands2/borderlands2.hpp>

class Borderlands2Test : public ::testing::Test {
protected:
    Borderlands2Test() {
        // You can do set-up work for each test here.
    }

    ~Borderlands2Test() override {
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

TEST_F(Borderlands2Test, VerifySaveFile) {
    EXPECT_TRUE(verifySave("../../resources/76561198034853688/Save0001.sav"));
}

TEST_F(Borderlands2Test, InvalidPath) {
    EXPECT_THROW(verifySave("./../resources/76561198034853688/Save0001.sav"), boost::filesystem::filesystem_error);
    EXPECT_THROW(verifySave("./../resources/76561198034853688/Save0001.sav"), boost::filesystem::filesystem_error);
}