#include "binary_search.hpp"
#include "debug.hpp"

using namespace mrsuyi;

int main() {
  int nums[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  assert(*lower_bound(nums, nums + 9, 4) == 4);
  assert(*lower_bound(nums, nums + 9, 6) == 6);

  assert(*upper_bound(nums, nums + 9, 4) == 5);
  assert(*upper_bound(nums, nums + 9, 6) == 7);

  return 0;
}
