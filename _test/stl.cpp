#include <iostream>
#include <utility>
#include <memory>
#include <vector>
#include <type_traits>

using namespace std;

int main()
{
    vector<int> b;

    cout << is_trivially_destructible<int>::value << endl;

    return 0;
};

