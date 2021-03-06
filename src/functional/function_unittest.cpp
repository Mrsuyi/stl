#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "function.hpp"

using namespace mrsuyi;

namespace {

int add(int& a) {
  return ++a;
}
struct Class {
  int add(int& a) { return ++a; }
};

}  // namespace

TEST(FunctionTest, ConstructorAndInvoke) {
  // default constructor
  function<void(void)> f;
  // from function pointer
  function<int(int&)> func_ptr(add);
  // from member function pointer
  function<int(Class&, int&)> mem_func_ptr(&Class::add);
  // from lambda
  function<int(int&)> lambda([](int& a) { return ++a; });
  // copy constructor
  function<int(int&)> cp(lambda);

  // invoke
  Class c;
  int num = 0;
  EXPECT_EQ(1, func_ptr(num));
  EXPECT_EQ(1, num);
  EXPECT_EQ(2, lambda(num));
  EXPECT_EQ(2, num);
  EXPECT_EQ(3, cp(num));
  EXPECT_EQ(3, num);
  EXPECT_EQ(4, mem_func_ptr(c, num));
  EXPECT_EQ(4, num);
}

TEST(FunctionTest, CheckValid) {
  function<int(int&)> empty;
  EXPECT_FALSE(empty);
  function<int(int&)> valid([](int& a) { return ++a; });
  EXPECT_TRUE(valid);
}
