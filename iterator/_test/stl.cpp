#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main()
{
    vector<int> a = {1, 2, 3};

    auto it = a.begin();
    advance(it, 1);

    return 0;
};

