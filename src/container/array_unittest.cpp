#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "array.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(ArrayTest, Ctor)
{
  array<int, 3> ary = {1, 2, 3};
  EXPECT_THAT(ary, ElementsAre(1, 2, 3));
}
