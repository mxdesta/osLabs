#include <gtest/gtest.h>
#include "child.h"

TEST(IsValidStringTest, EmptyString) {
    EXPECT_FALSE(is_valid_string(""));
}


TEST(IsValidStringTest, StartsWithUppercase) {
    EXPECT_TRUE(is_valid_string("Hello"));
}


TEST(IsValidStringTest, StartsWithLowercase) {
    EXPECT_FALSE(is_valid_string("hello"));
}


TEST(IsValidStringTest, StartsWithNumber) {
    EXPECT_FALSE(is_valid_string("123abc"));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
