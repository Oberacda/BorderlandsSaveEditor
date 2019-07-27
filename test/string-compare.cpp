//
// Created by David Oberacker on 2019-07-22.
//

#include <gtest/gtest.h> // googletest header file

#include <string>
using std::string;

const char *actualValTrue  = "hello gtest";
const char *actualValFalse = "hello world";
const char *expectVal      = "hello gtest";

TEST(StrCompare, CStrEqual) {
    EXPECT_STREQ(expectVal, actualValTrue);
}


TEST(StrCompare, DISABLED_CStrNotEqual) {
    EXPECT_STREQ(expectVal, actualValFalse);
}