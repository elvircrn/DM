#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>

std::vector<double> pA = { 0.4, 0.44999999999999996, 0.15 };
std::vector<double> pB ;

std::vector<std::vector<double>> mat = {
    { 0.3, 0.1, 0.05, 0.5 },
    { 0.25, 0.3, 0.25, 0.1, 0.1 },
    { 0.1, 0.15, 0.35, 0.25 }
};

std::vector<double> pAB;

int main()
{
    double HXY = 0.0;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 3; i++)
            HXY += pA[i] * mat[i][j] * std::log2(pA[i] * mat[i][j]);

    // P (B_j)
    for (int j = 0; j < 4; j++)
    {
        pB.push_back(0.0);
        for (int i = 0; i < 3; i++)
            pB [j] += pA[i] * mat[i][j];
    }

    HXY *= -1.0;

    std::cout << "H(X, Y) = " << HXY << '\n';

    double HYIX = 0.0;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 3; i++)
            HYIX += pA[i] * mat[i][j] * std::log2(mat[i][j]);
    HYIX *= -1.0;
    std::cout << "H(Y|X) = " << HYIX << '\n';

    double HXIY = 0.0;
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 3; i++)
            HXIY += pA[i] * mat[i][j] * std::log2(mat[i][j]);
    HXIY *= -1.0;
    std::cout << "H(X|Y) = " << HXIY << '\n';

    double HY = 0.0;
    for (int j = 0; j < 4; j++)
        HY += pB[j] * std::log2(pB[j]);
    HY *= -1.0;
    std::cout << "H(Y) = " << HY << '\n';


    double HX = HXY - HYIX;

    std::cout << "H(X) = " << HX << '\n';

    double IXY = HY - HYIX;

    std::cout << "I(X, Y) = " << IXY << '\n';

    std::cout << "XHY check " << HX + HYIX << ' ' << HXY << '\n';

    return 0;
}
