#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "stack.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(StackTest, PushPop) {
  stack<int> stk;
  stk.push(1);
  EXPECT_EQ(1, stk.top());
  EXPECT_EQ(1, stk.size());
  stk.push(2);
  stk.push(3);
  EXPECT_EQ(3, stk.top());
  EXPECT_EQ(3, stk.size());
  stk.pop();
  EXPECT_EQ(2, stk.top());
  EXPECT_EQ(2, stk.size());
  stk.pop();
  stk.pop();
  EXPECT_EQ(0, stk.size());
}
