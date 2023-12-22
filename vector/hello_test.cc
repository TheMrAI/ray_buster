#include <gtest/gtest.h>

#include "vector.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(SayHelloTest, Basic) {
  EXPECT_EQ(std::string("hello"), say_hello());
}
