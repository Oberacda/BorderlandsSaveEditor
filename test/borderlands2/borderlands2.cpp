//
// Created by David Oberacker on 2019-08-02.
//

#include <gtest/gtest.h>
#include <borderlands2/borderlands2.hpp>

TEST(Borderlands2, VerifySaveFile) {
    EXPECT_TRUE(verifySave("./resources/76561198034853688/Save0001.sav"));
}