#include <iostream>
#include <string>
using namespace std;

auto main() -> int
{
    auto add_two([](auto x, auto y){ return x + y; });

    cout << add_two("I"s, "t"s) << " works!" << endl;
}

