#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

double dp [100] [100] [123], prob = 0.46;

double f(bool flipped, int i, int start)
{
    if (i == start)
        return !flipped ? prob : (1 - prob);
    return f(flipped, i - 1, start) * prob + f(!flipped, i - 1, start) * (1 - prob);
}



int main()
{
    memset(dp, -1, sizeof dp);

    for (int i = 1; i <= 4; i++)
        std::cout << f(0, i, 1) << ' ';
    std::cout << '\n';
    for (int i = 2; i <= 10; i++)
        std::cout << f(0, i, 2) << ' ';

    std::cout << '\n';

    std::cout << f(0, 2, 1) << '\n';
    std::cout << f(0, 4, 1) << '\n';

    std::cout << (f(0, 2, 1) * f(0, 2, 2)) / f(0, 4, 1) << '\n';


    return 0;
}
