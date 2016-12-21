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
std::vector<std::pair<T, double>> getProb(const std::vector<std::pair<T, int>> &in)
{
    std::vector<std::pair<T, double>> v;
    int total = 0;
    for (auto &x : in)
        total += x.second;
    for (auto &x : in)
        v.emplace_back(x.first, (double)x.second / total);
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

template<class T>
std::map<T, std::string> shannonFano(std::vector<std::pair<T, double>> p)
{
    std::map<T, std::string> res;
    std::sort(p.begin(), p.end(), [](std::pair<T, double> &a, std::pair<T, double> &b) -> bool { return a.second > b.second; });

    double total = 1.0;

    typedef std::pair<double, std::vector<T>> Data;
    std::set<T> charSet;

    std::priority_queue<Data, std::vector<Data>, bool (*)(const Data &a, const Data &b)> q ([](const Data &a, const Data &b) -> bool { return a.first > b.first; });
    
    for (auto& x : p)
    {
        q.push(Data(x.second, { x.first }));
        charSet.insert(x.first);
    }

    while (!q.empty())
    {
        double cnt = 0.0;
        double totalCpy = total;

        std::vector<T> s;
        
        while (!q.empty() && 2 * cnt < totalCpy)
        {
            Data d = q.top();
            q.pop();

            for (auto& c : d.second)
            {
                std::cout << c << ' ';
                res[c] += '0';
                charSet.erase(charSet.find(c));
            }

            std::putchar('\n');

            total -= d.first;
            cnt += d.first;
        }

        for (auto& c : charSet)
            res[c] += '1';
    }

    return res;
}

template<class T>
std::map<T, std::string> getShannonFano(const std::vector<std::pair<T, int>> &p)
{
    return shannonFano<T>(getProb<T>(p));
}

int main()
{
    /*
    std::vector<int> v = { 36, 85, 28, 51, 71, 76, 91, 82, 29, 30 };
    std::vector<std::pair<char, int>> p;

    for (int i = 0; i < v.size(); i++)
        p.push_back(std::make_pair('A' + i, v[i]));

    auto m = getShannonFano<char>(p);

    for (auto& x : m)
        std::cout << x.first << ' ' << x.second << '\n';
    */

    std::vector<std::pair<double, std::string>> data;

    std::vector<double> p = { 0.7, 0.2, 0.1 };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data.emplace_back(p[i] * p[j], "x_" + patch::to_string(i + 1) + "x_" + patch::to_string(j + 1));
        }
    }

    std::sort(data.begin(), data.end());

    std::reverse(data.begin(), data.end());

    std::vector<double> prefix(data.size());

    prefix[0] = data[0].first;

    for (int i = 1; i < prefix.size(); i++)
        prefix[i] = prefix[i - 1] + data[i].first;

    std::map<std::string, std::string> m;

    solve<std::string>(0, (int)data.size() - 1, prefix, m, data);

    for (int i = 0; i < data.size(); i++)
        std::cout << data[i].first << ' ' << data[i].second << '\n';

    std::putchar('\n');

    for (int i = 0; i < data.size(); i++)
        std::cout << data[i].second << ' ' << m[data[i].second] << '\n';


    /* analyze */
    double nsr = 0.0, hinf = 0.0;
    for (auto& d : data)
    {
        nsr += d.first * d.second.length();
        hinf -= d.first * std::log2(d.first);
    }

    std::cout << "nsr = " << nsr << '\n';
    std::cout << "hinf = " << hinf << '\n';
    std::cout << "brzina prenosa = " << hinf / nsr << '\n';
    std::cout << "procenat iskoristnosti = " << (hinf / nsr) * 100.0 << "%\n";

    return 0;
}












