#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <sstream>

void begin(std::string s)
{
    std::cout << "\\begin{" << s << "}";
}

void end(std::string s)
{
    std::cout << "\\end{" << s << "}";
}

template<class T>
void DrawVector(const std::vector<T> &v, std::string sep = "")
{
    std::cout << v[0];
    for (int i = 1; i < v.size(); i++)
        std::cout << sep << v[i];
}

namespace patch
{
    template<class T>
    std::string ToString(const T &x)
    {
        std::stringstream ss;
        ss << x;
        return ss.str();
    }
}

std::vector<std::string> GenIt(int n)
{
    std::vector<std::string> ret;
    for (int i = 1; i <= n; i++)
        ret.push_back("Iteration " + patch::ToString(i));
    return ret;
}

template<class T>
void DrawTable(const std::vector<std::string> &columnNames,
        const std::vector<std::vector<T>> &mat,
        const std::string &topLeft)
{
    putchar('\n');
    putchar('\n');

    begin("tabular");
    std::cout << "{ | c || c ";
    for (int i = 1; i < columnNames.size(); i++)
        std::cout << " | c";
    std::cout << " | } \n";

    std::cout << "\\hline\n";
    // Draws the column names
    std::cout << topLeft << " & ";
    DrawVector(columnNames, " & ");
    std::cout << "\\\\\n \\hline\n \\hline \n";

    for (int i = 0; i < mat.size(); i++)
    {
        std::cout << "Iteracija " << i + 1 << " & ";
        DrawVector(mat[i], " & ");
        std::cout << "\\\\\n \\hline\n";
    }
    end("tabular");
}

template<class T>
class Edge
{
public:
    T u, v;
    int w;

    Edge() { }
    Edge(const T &_v, int _w) : v(_v), w(_w) { }
    Edge(const T &_u, const T &_v, int _w) : u(_u), v(_v), w(_w) { }

    bool operator< (const Edge &e) const
    {
        return w > e.w;
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << "(" << u << ", " << v << ", " << w << ")";
        return ss.str();
    }
};

template<class T>
class Graph
{
    std::map<T, std::vector<Edge<T>>> g;
    std::set<T> vertices;
    std::vector<Edge<T>> edges;
public:

    void AddEdgeDir(const T &u, const T &v, int w)
    {
        edges.emplace_back(u, v, w);
        g[v].emplace_back(u, v, w);
        vertices.insert(u);
        vertices.insert(v);
    }

    void AddEdge(const T &u, const T &v, int w)
    {
        AddEdgeDir(u, v, w);
        AddEdgeDir(v, u, w);
    }

    std::vector<Edge<T>> GetEdges() const
    {
        return edges;
    }

    std::set<T> GetVertices() const
    {
        return vertices;
    }

    int V() const
    {
        return (int)vertices.size();
    }

    int E() const
    {
        return (int)edges.size();
    }
};

template<class T>
void DrawEdges(const std::vector<Edge<T>> &s, const std::string &name = "")
{
    if (name != "")
        std::cout << name << " = ";
    std::cout << " { ";
    std::cout << s[0].ToString();
    for (int i = 1; i < s.size(); i++)
        std::cout << ", " << s[i].ToString();
    std::cout << " }";
}

template<class T>
class KruskalSolver
{
    std::map<T, T> parent;
    std::map<T, int> count;
    Graph<T> g;

    T Find(const T& x)
    {
        if (parent[x] == x)
            return x;
        else
            return parent[x] = Find(parent[x]);
    }

    bool Union(const T& x, const T& y)
    {
        T pX = Find(x), pY = Find(y);

        if (pX != pY && count[pX] > count[pY])
        {
            parent[pY] = pX;
            count[pX] += count[pY];
            return true;
        }
        else if (pX != pY && count[pX] <= count[pY])
        {
            parent[pX] = pY;
            count[pY] += count[pX];
            return true;
        }
        return false;
    }

    std::vector<std::vector<T>> mat;

    Graph<T> tree;

public:
    KruskalSolver(const Graph<T> &_g) : g(_g)
    {
        auto edges = g.GetEdges();
        std::sort(edges.begin(), edges.end());
        
        DrawEdges(edges, "G");

        for (auto& v : g.GetVertices())
        {
            parent[v] = v;
            count[v] = 1;
        }

        int it = 0;

        for (auto& e : g.GetEdges())
        {
            mat.push_back(std::vector<T>());
            if (Union(e.u, e.v))
                tree.AddEdge(e.u, e.v, e.w);

            for (auto& v : g.GetVertices())
                mat[it].push_back(Find(parent[v]));

            if (g.V() - 1 == tree.E() / 2)
                break;
            it++;
        }
    }

    KruskalSolver<T> Draw()
    {
        auto v = tree.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                mat, "Iteracija");
    }
};

int main()
{
    freopen("drugi.txt", "r", stdin);
    int u, v, w;
    Graph<std::string> g;
    while (scanf("(L%d, L%d, %d)", &u, &v, &w) == 3)
    {
        std::string base = "\\(L_{";
        g.AddEdge(base + patch::ToString(u) + "}\\)", base + patch::ToString(v) + "}\\)", w);
        for (int i = 0; i < 3; i++)
            getchar();
    }

    KruskalSolver<std::string>(g).Draw();
    std::cout << '\n';

    return 0;
}


































