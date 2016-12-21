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


int bin[DIM][DIM];
int qqq[DIM][DIM];
int s[DIM][DIM];

int binom(int n, int k)
{
    if (bin[n] [k] != -1)
        return bin[n][k];
    if (k == 0 || n == k)
        return bin[n][k] = 1;
    return bin[n][k] = binom(n - 1, k - 1) + binom(n - 1, k);

}

int s_recursive(int n,int k)
{
    if (s [n] [k] != -1)
        return s [n] [k];

    if (k == 1 || k == n)
        return s [n] [k] = 1;

    return s [n] [k] = s_recursive(n-1,k-1) + k*s_recursive(n-1,k);
}

int c_over(int n, int k)
{
    return binom(n + k - 1, n);
}

int get_q(int n, int k)
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

string render_stirling(int n, int k)
{
    return "S_{" + to_string(n) + "}^{" + to_string(k) + "}";
}

string render_c(int n, int k)
{
    return "C_{" + to_string(n) + "}^{" + to_string(k) + "}";
}

void init()
{
    memset(s, -1, sizeof s);
    memset(bin, -1, sizeof bin);
    memset(qqq, -1, sizeof qqq);
}

int fast_pow(int base, int power)
{
    if (power == 0)
        return 1;
    else if (power == 1)
        return base;
    else if (power % 2 == 0)
    {
        int t = fast_pow(base, power / 2);
        return t * t;
    }
    else
        return fast_pow(base, power - 1) * base;
}

string frac(string a, string b)
{
    return "\\frac{" + a + "}{" + b + "}";
}

string frac(int a, int b)
{
    return frac(to_string(a), to_string(b));
}

std::vector<double> T = { 0.5, 0.45, 0.65, 0.4 };

double get(int enc)
{
    double ret = 0.0;
    for (int i = 0; i < 4; i++)
        if (enc & (1<<i))
            ret += T[i];
        else
            ret += (1 - T[i]);
    return ret;
}

int toggle(int enc, int i)
{
    return enc ^ (1<<i);
}

std::string render(int enc)
{
    string ret = "P (";
    for (int i = 0; i < 4; i++)
        if (enc & (1<<i))
            ret += "T_" + to_string(i + 1);
        else
            ret += "\\overbar{T}_" + to_string(i + 1);
    return ret + ")";
}

double calc(int enc)
{
    double ret = 1.0;
    for (int i = 0; i < 4; i++)
        if (enc & (1<<i))
            ret *= T[i];
        else
            ret *= (1.0 - T[i]);
    return ret;
}

int main()
{
    init();

    std::cout << "b)\n";

    std::cout << render_c(22, 1) << ' ' << render_c(22, 1) << ' ' << render_c(22, 2) << '\n';

    std::cout << frac(render_c(22, 1) + " \\cdot " + render_c(22, 1) + " + " + render_c(22, 2), render_c(94, 2)) << '\n';

    double x = binom(22, 1) * binom(22, 1) + binom(22, 2);
    double y = binom(94, 2);

    std::cout << 1 - x / y << '\n';


    std::cout << "c)\n";

    std::cout << frac(render_c(22, 1) + " \\cdot " + render_c(22, 2), render_c(94, 3)) << '\n';

    x = binom(22, 1) * binom(22, 2);
    y = binom(94, 3);

    std::cout << 1 - x / y << '\n';


    return 0;
}






















