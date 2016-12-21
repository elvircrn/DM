#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cstring>
#include <sstream>

#define DIM 1000

using namespace std;

std::string to_string(long i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}


unsigned long long bin[DIM][DIM];
unsigned long long qqq[DIM][DIM];
unsigned long long s[DIM][DIM];

unsigned long long binom(int n, int k)
{
    if (bin[n] [k] != -1)
        return bin[n][k];
    if (k == 0 || n == k)
        return bin[n][k] = 1;
    return bin[n][k] = binom(n - 1, k - 1) + binom(n - 1, k);

}

unsigned long long s_recursive(unsigned long long n,unsigned long long k)
{
    if (s [n] [k] != -1)
        return s [n] [k];

    if (k == 1 || k == n)
        return s [n] [k] = 1;

    return s [n] [k] = s_recursive(n-1,k-1) + k*s_recursive(n-1,k);
}

unsigned long long c_over(unsigned long long n, unsigned long long k)
{
    return binom(n + k - 1, n);
}

unsigned long long get_q(unsigned long long n, unsigned long long k)
{
    if (qqq[n][k] != -1)
        return qqq[n][k];
    if (k == 1 || n == 0)
        return qqq[n] [k] = 1;
    if (k > n)
        return qqq[n] [k] = get_q(n, n);

    int x = get_q(n, k - 1) + get_q(n - k, k);


    qqq[n] [k] = x;
    qqq[n] [n] = x;

    return x;
}

string render_stirling(unsigned long long n, unsigned long long k)
{
    return "S_{" + to_string(n) + "}^{" + to_string(k) + "}";
}

string render_c(unsigned long long n, unsigned long long k)
{
    return "C_{" + to_string(n) + "}^{" + to_string(k) + "}";
}

void init()
{
    memset(s, -1, sizeof s);
    memset(bin, -1, sizeof bin);
    memset(qqq, -1, sizeof qqq);
}

unsigned long long fast_pow(unsigned long long base, unsigned long long power)
{
    if (power == 0)
        return 1;
    else if (power == 1)
        return base;
    else if (power % 2 == 0)
    {
        unsigned long long t = fast_pow(base, power / 2);
        return t * t;
    }
    else
        return fast_pow(base, power - 1) * base;
}

int main()
{
    init();

    unsigned long long x = 0;

    std::cout << "\\frac{";

    for (int i = 25; i <= 39; i++)
        std::cout << render_c(64, i) << " + ";
    std::cout << '\n';
    for (int i = 25; i <= 39; i++)
        x += binom(64, i);

    std::cout << "}{2^{64} = ";


    unsigned long long y = fast_pow(2, 63);
    x /= 2;



    std::cout << "\\frac{" << x << "}{" << y << "} \\approx ";
    std::cout << ((double)x) / y << '\n';

    return 0;
}















