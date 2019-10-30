//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <borderlands2/borderlands2.hpp>
#include <google/protobuf/stubs/common.h>

class Borderlands2Test : public ::testing::Test {
    public:

    D4v3::Borderlands::Borderlands2::Borderlands2_Save_File save_file = D4v3::Borderlands::Borderlands2::Borderlands2_Save_File(
            "./borderlands2/resources/76561198034853688/Save0001.sav");

    Borderlands2Test() {
    }

    ~Borderlands2Test() override {
        // You can do clean-up work that doesn't throw exceptions here.
        google::protobuf::ShutdownProtobufLibrary();
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
    EXPECT_TRUE(save_file.verifySave());
}

TEST_F(Borderlands2Test, InvalidPath) {
    EXPECT_ANY_THROW(D4v3::Borderlands::Borderlands2::Borderlands2_Save_File("./../resources/76561198034853688/Save0001.sav"));
}

TEST_F(Borderlands2Test, InvalidPath_InvalidChars) {
    EXPECT_ANY_THROW(D4v3::Borderlands::Borderlands2::Borderlands2_Save_File("..//../resources/76561198034853688/Save0001.sav"));
}

TEST_F(Borderlands2Test, DISABLED_DumpJson) {
    EXPECT_NO_THROW(save_file.dumpSave("./Save0001.dump.json"));
}
