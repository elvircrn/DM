#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

namespace patch
{
    template<class T> std::string to_string(const T &n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

template<class T>
std::vector<std::pair<double, T>> getProb(const std::vector<std::pair<T, int>> &in)
{
    std::vector<std::pair<double, T>> v;
    int total = 0;
    for (auto &x : in)
        total += x.second;
    for (auto &x : in)
        v.emplace_back((double)x.second / total, x.first);
    return v;
}

double getSum(const std::vector<double> &prefix, int left, int right)
{
    return prefix[right] - (!left ? 0 : prefix[left - 1]);
}

int findIndex(const std::vector<double> &prefix, int left, int right)
{
    if (right - left < 3)
        return left;

    double total = getSum(prefix, left, right);

    int l = left, r = right, pivot;

    while (l < r)
    {
        pivot = (l + r) / 2;
        if (getSum(prefix, l, pivot) * 2 < total)
            l = pivot + 1;
        else
            r = pivot - 1;
    }

    return l;
}

int depth = 0;

template<class T>
void solve(int left, int right, const std::vector<double> &prefix, 
                                std::map<T, std::string> &m,
                                const std::vector<std::pair<double, T>> &data,
                                char color = '0')
{
    for (int i = 0; i < depth; i++)
        std::putchar('-');
    depth++;
    if (left == right)
    {
        m[data[left].second] += color;
        std::cout << data[left].second << '=' << m[data[left].second] << '\n';
        depth--;
        return;
    }
    //std::cout << m[data[left].second] << '\n';

    int index = findIndex(prefix, left, right);

    if (left != index)
        for (int i = left; i <= index; i++)
            m [data[i].second] += '0';

    if (right != index + 1)
        for (int i = index + 1; i <= right; i++)
            m [data[i].second] += '1';

    std::cout << getSum(prefix, left, index) << " + " << getSum(prefix, index + 1, right) << '\n';

    solve(left, index, prefix, m, data, '0');
    solve(index + 1, right, prefix, m, data, '1');

    depth--;
}

int main()
{
    std::vector<int> v = { 36, 85, 28, 51, 71, 76, 91, 82, 29, 30 };
    std::vector<std::pair<std::string, int>> p;
    std::string word = "A";
    for (int i = 0; i < v.size(); i++)
    {
        p.emplace_back(word, v[i]);
        word[0]++;
    }
    std::vector<std::pair<double, std::string>> data = getProb<std::string>(p);

    std::cout << "\\{ ";
    for (auto& elem : data)
        std::cout << "(" << elem.second << ", " << elem.first << "), ";
    std::cout << '\n';


    std::sort(data.begin(), data.end());

    std::reverse(data.begin(), data.end());

    std::vector<double> prefix(data.size());

    prefix[0] = data[0].first;

    for (int i = 1; i < prefix.size(); i++)
        prefix[i] = prefix[i - 1] + data[i].first;

    std::map<std::string, std::string> m;

    solve<std::string>(0, (int)data.size() - 1, prefix, m, data);
    
    std::cout << "\n\n";
    for (auto& elem : m)
        std::cout << elem.first << " &\\rightarrow " << elem.second << "\\\\\n";
    std::cout << "\n\n";

    for (int i = 0; i < data.size(); i++)
        std::cout << data[i].first << ' ' << data[i].second << '\n';

    std::putchar('\n');

    for (int i = 0; i < data.size(); i++)
        std::cout << data[i].second << ' ' << m[data[i].second] << '\n';


    /* analyze */
    double nsr = 0.0, hinf = 0.0;
    for (auto& d : data)
    {
        nsr += d.first * m[d.second].length();
        hinf -= d.first * std::log2(d.first);
    }

    std::cout << "nsr = " << nsr << '\n';
    std::cout << "hinf = " << hinf << '\n';
    std::cout << "brzina prenosa = " << hinf / nsr << '\n';
    std::cout << "procenat iskoristnosti = " << (hinf / nsr) * 100.0 << "%\n";

    std::string text = "FCJJCGHAFAEI";
    for (char c : text)
    {
        std::string s; s += c;
        std::cout << m[s];
    }
    std::cout << '\n';
 
    return 0;
}












