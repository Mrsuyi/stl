#include "pair.hpp"
#include <iostream>
#include <utility>
#include "gtest/gtest.h"

using namespace mrsuyi;

TEST(PairTest, Ctor) {
  pair<int, int> p(1, 2);
  EXPECT_EQ(p.first, 1);
  EXPECT_EQ(p.second, 2);
}

TEST(PairTest, Assign) {
  pair<int, int> p(1, 2);
  p = {3, 4};
  EXPECT_EQ(p.first, 3);
  EXPECT_EQ(p.second, 4);
}
