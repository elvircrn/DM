#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <functional>
#include <utility>

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
class HuffmanTree
{
private:
    typedef std::pair<T, int> Node;
    typedef std::pair<double, T> Data;

    struct Edge
    {
        T v;
        char c;

        Edge() { }
        Edge(const Node &_v, char _c) : v(_v), c(_c) { }
    };


    std::unordered_map<T, T> parent;
    std::unordered_map<T, double> prob;
    std::map<Node, std::vector<Node>> graph;
    std::vector<char> codes;
    std::unordered_map<T, int> groupSize;

    Node root;

    std::unordered_map<T, std::string> huffman;

    int TakeCount(int n) const
    {
        return 2 + ((n - 4) % (codes.size() - 1));
    }

    void InitValues(const std::vector<std::pair<T, double>> &dataSet)
    {
        for (auto& x : dataSet)
        {
            prob[x.first] = x.second;
            parent[x.first] = x.first;
            groupSize[x.first] = 1;
        }
    }

    T Find(const T &x)
    {
        if (parent[x] == x)
            return x;
        else
            return parent[x] = Find(parent[x]);
    }

    Node FindNode(const T &x)
    {
        return Node(x, groupSize[x]);
    }

    Node Union(const T &x, const T &y)
    {
        T fX = Find(x);
        T fY = Find(y);

        if (groupSize[fX] < groupSize[fY])
        {
            groupSize[fY] += groupSize[fX];
            parent [fX] = fY;
            return Node(fY, groupSize[fY]);
        }
        else
        {
            groupSize[fX] += groupSize[fY];
            parent [fY] = fX;
            return Node(fX, groupSize[fX]);
        }
    }

    void Huffman()
    {
        std::priority_queue<Data,
                            std::vector<Data>,
                            std::function<bool(const Data &a, const Data &b)>> pq (
                                    [](const Data &a, const Data &b) -> bool
                                    {
                                        return a.first > b.first;
                                    });

        for (auto& x : prob)
            pq.push(Data(x.second, x.first));

        Node node;
        while (pq.size() > 1)
        {
            int m = TakeCount((int)pq.size());

            auto mainElem = pq.top();
            pq.pop();

            Node lastNode = FindNode(mainElem.second);

            double count = mainElem.first;
            std::vector<Node> nodes(m - 1); // TODO: Optimize

            for (int i = 0; i < m - 1; i++)
            {
                auto help = pq.top();
                pq.pop();
                count += help.first;

                nodes [i] = FindNode(help.second);
                node = Union(help.second, mainElem.second);
            }

            nodes.push_back(lastNode);

            for (Node& n : nodes)
                graph[node].push_back(n);


            pq.push(Data(count, node.first));
        }

        root = node;
    }

    void Traverse(const Node &current, std::string buffer, int depth = 0, bool write = false)
    {
        depth++;

        if (write)
            for (int i = 0; i < depth; i++)
                std::putchar('-');

        if (graph[current].size() && write)
        {
            for (int i = 0; i < graph[current].size() - 1; i++)
                std::cout << prob[graph[current][i].first] << " + ";
            std::cout << prob[graph[current].back().first] << '\n';
        }
        
        int index = 0;
        for (auto& next : graph[current])
            Traverse(next, buffer + codes[index++], depth, write);

       if (!graph[current].size())
       {
           huffman[current.first] = buffer;
           if (write)
               std::cout << current.first << "$\\rightarrow$ " << buffer << '\n';
       }
    }

public:
    static std::vector<std::pair<T, double>> GetProb(const std::vector<std::pair<T, int>> &in)
    {
        std::vector<std::pair<T, double>> v;
        int total = 0;
        for (auto &x : in)
            total += x.second;
        for (auto &x : in)
            v.emplace_back(x.first, (double)x.second / total);
        return v;
    }

    HuffmanTree(const std::vector<std::pair<T, double>> &dataSet,
                const std::vector<char> &_codes) : codes(_codes)
    {
        InitValues(dataSet);
        Huffman();
    }

    void Join(bool write = false)
    {
        if (write)
            std::cout << ".-\n";
        Traverse(root, "", 0, write);
    }

    void Analyse()
    {
        std::unordered_map<T, int> n;

        for (auto &e : huffman)
            n[e.first] = e.second.length();

        double nsr = 0.0;
        for (auto &e : huffman)
            nsr += prob[e.first] * n[e.first];

        std::cout << "nsr = " << nsr << '\n';

        double hinf = 0.0;
        for (auto &e : huffman)
            hinf -= prob[e.first] * std::log2(prob[e.first]);

        std::cout << "hinf = " << hinf << '\n';

        std::cout << "brzina prenosa = " << hinf / nsr << '\n';

        std::cout << "procenat iskoristenosti = " << (hinf / nsr) * 100.0 << "%\n";
    }
};

int main()
{
    HuffmanTree<std::string> test ({{ "x3", 0.25 },
                                    { "x6", 0.25 },
                                    { "x1", 0.2 },
                                    { "x5", 0.15 },
                                    { "x4", 0.1 },
                                    { "x2", 0.05 }
                                    }, { '0', '1' });

    test.Join(true);
    test.Analyse();

    return 0;
}















