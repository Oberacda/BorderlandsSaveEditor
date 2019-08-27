//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions/formatters/stream.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/attr_fwd.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

// Supporting headers
#include <boost/log/support/date_time.hpp>


#include <borderlands2/borderlands2.hpp>


class Borderlands2Test : public ::testing::Test {
    public:

    Borderlands2Test() {
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
    EXPECT_TRUE(verifySave(".\\borderlands2\\resources\\76561198034853688\\Save0001.sav"));
}

TEST_F(Borderlands2Test, InvalidPath) {
    EXPECT_ANY_THROW(verifySave("./../resources/76561198034853688/Save0001.sav"));
}

TEST_F(Borderlands2Test, InvalidPath_InvalidChars) {
    EXPECT_ANY_THROW(verifySave("..//../resources/76561198034853688/Save0001.sav"));
}

TEST_F(Borderlands2Test, DumpJson) {
    EXPECT_NO_THROW(dumpSaveJson(".\\borderlands2\\resources\\76561198034853688\\Save0001.sav", ".\\Save0001.dump.json"));
    EXPECT_NO_THROW(dumpSaveJson(".\\borderlands2\\resources\\76561198034853688\\Save0007.sav", ".\\Save0007.dump.json"));
}
