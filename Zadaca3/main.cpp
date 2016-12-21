#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>

std::vector<double> pA = { 0.4, 0.44999999999999996, 0.15 };

std::vector<std::vector<double>> mat = {
    { 0.3, 0.1, 0.05, 0.5 },
    { 0.25, 0.3, 0.25, 0.1, 0.1 },
    { 0.1, 0.15, 0.35, 0.25 }
};

std::vector<double> pAB;

int main()
{
    double sum = 0.0;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 3; i++)
            std::cout << pA[i] * mat[i][j] * std::log2(pA[i] * mat[i][j]) << " + ";

            //sum += pA[i] * mat[i][j] * std::log2(pA[i] * mat[i][j]);

    std::cout << -sum << '\n';

    return 0;
}

























