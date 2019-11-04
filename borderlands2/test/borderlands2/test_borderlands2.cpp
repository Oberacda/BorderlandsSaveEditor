//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <borderlands2/save_file.hpp>
#include <google/protobuf/stubs/common.h>

class Borderlands2Test : public ::testing::Test {
    public:
    std::unique_ptr<D4v3::Borderlands::Borderlands2::Borderlands2_Save_File> saveFilePtr;

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
        auto newSaveFilePtr = std::make_unique<D4v3::Borderlands::Borderlands2::Borderlands2_Save_File>();
        this->saveFilePtr.swap(newSaveFilePtr);
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
        this->saveFilePtr.reset(nullptr);
    }
};

TEST_F(Borderlands2Test, VerifySaveFile) {
    EXPECT_NO_THROW(saveFilePtr->loadSaveFile("../borderlands2/resources/76561198034853688/Save0001.sav"));
    EXPECT_TRUE(saveFilePtr->verifySave());
}

TEST_F(Borderlands2Test, InvalidPath) {
    EXPECT_ANY_THROW(saveFilePtr->loadSaveFile("./../resources/76561198034853688/Save0001.sav"));
}

TEST_F(Borderlands2Test, InvalidPath_InvalidChars) {
    EXPECT_ANY_THROW(saveFilePtr->loadSaveFile("..//../resources/76561198034853688/Save0001.sav"));
}

