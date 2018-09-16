#include "gtest/gtest.h"

#include "iterator/functions.hpp"

using namespace mrsuyi;

TEST(IteratorFunctionTest, Advance) {
  int nums[] = {1, 2, 3, 4, 5, 6};
  int* a = nums;
  advance(a, 1);
  EXPECT_EQ(nums + 1, a);
  advance(a, 5);
  EXPECT_EQ(nums + 6, a);
}
