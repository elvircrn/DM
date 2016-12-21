#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

int


 main()
{
    long long a = 1;
    for (long long i = 165; i <= 170; i++)
        a *= i;

    long long b = 1;
    for (long long i = 175; i <= 180; i++)
        b *= i;


    std::cout << (a / __gcd(a, b)) << ' ' << (b / __gcd(a, b)) << '\n';

    std::cout << a / (double)b - 1. << '\n';


    return 0;
}
