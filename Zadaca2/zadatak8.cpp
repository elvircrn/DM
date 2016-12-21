#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;

std::string to_string(long i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}


long long bin[100][100];
long long qqq[100][100];

int binom(int n, int k)
{
    if (bin[n] [k] != -1)
        return bin[n][k];
    if (n == 0 || n == k)
        return bin[n][k] = 1;
    return bin[n][k] = binom(n - 1, k - 1) + binom(n - 1, k);

}

long long s[100][100];

long long s_recursive(long long n,long long k)
{
    if (s [n] [k] != -1)
        return s [n] [k];

    if (k == 1 || k == n)
        return s [n] [k] = 1;

    return s [n] [k] = s_recursive(n-1,k-1) + k*s_recursive(n-1,k);
}

long long c_over(long long n, long long k)
{
    return binom(n + k - 1, n);
}

long long get_q(long long n, long long k)
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

string render_stirling(long long n, long long k)
{
    return "S_{" + to_string(n) + "}^{" + to_string(k) + "}";
}

void init()
{
    memset(s, -1, sizeof s);
    memset(bin, -1, sizeof bin);
    memset(qqq, -1, sizeof qqq);
}

int main()
{
    init();
    std::cout << get_q(11, 3) << '\n';
    return 0;
}














