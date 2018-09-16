#include "gtest/gtest.h"

#include "permutation.hpp"

using namespace mrsuyi;

TEST(PermutationTest, IsPermutation) {
  int a[] = {1, 2, 2, 4, 4};
  int b[] = {4, 2, 4, 2, 1};
  int c[] = {4, 2, 4, 2, 3};

  EXPECT_TRUE(is_permutation(a, a + 5, b));
  EXPECT_FALSE(is_permutation(b, b + 5, c));
}
