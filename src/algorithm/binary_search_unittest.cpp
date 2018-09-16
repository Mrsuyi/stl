#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "binary_search.hpp"
#include "iterator.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(BinarySearchTest, LowerBound) {
  int nums[] = {1, 2, 3, 4, 6, 6, 7, 8, 9};
  EXPECT_EQ(1, *lower_bound(nums, nums + 9, -1));
  EXPECT_EQ(4, *lower_bound(nums, nums + 9, 4));
  EXPECT_EQ(6, *lower_bound(nums, nums + 9, 5));
  EXPECT_EQ(9, *lower_bound(nums, nums + 9, 9));
  EXPECT_EQ(size(nums), lower_bound(nums, nums + 9, 100) - nums);
}

TEST(BinarySearchTest, UpperBound) {
  int nums[] = {1, 2, 3, 4, 6, 6, 7, 8, 9};
  EXPECT_EQ(1, *upper_bound(nums, nums + 9, -1));
  EXPECT_EQ(6, *upper_bound(nums, nums + 9, 4));
  EXPECT_EQ(6, *upper_bound(nums, nums + 9, 5));
  EXPECT_EQ(size(nums), upper_bound(nums, nums + 9, 9) - nums);
  EXPECT_EQ(size(nums), upper_bound(nums, nums + 9, 100) - nums);
}

TEST(BinarySearchTest, BinarySearch) {
  int nums[] = {1, 2, 3, 4, 6, 6, 7, 8, 9};
  EXPECT_EQ(3, *binary_search(nums, nums + 9, 3));
  EXPECT_EQ(6, *binary_search(nums, nums + 9, 6));
  EXPECT_EQ(size(nums), binary_search(nums, nums + 9, 5) - nums);
}

TEST(BinarySearchTest, EqualRange) {
  int nums[] = {1, 2, 3, 4, 6, 6, 7, 8, 9};
  EXPECT_THAT(equal_range(nums, nums + 9, -1), Pair(nums, nums));
  EXPECT_THAT(equal_range(nums, nums + 9, 1), Pair(nums, nums + 1));
  EXPECT_THAT(equal_range(nums, nums + 9, 6), Pair(nums + 4, nums + 6));
  EXPECT_THAT(equal_range(nums, nums + 9, 100), Pair(nums + 9, nums + 9));
}
