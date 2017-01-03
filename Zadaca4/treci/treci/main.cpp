#include <iostream>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <map>
#include <set>
#include <vector>
#include <limits>
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
    for (size_t i = 1; i < v.size(); i++)
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

    int ToInt(const std::string &s)
    {
        int x;
        std::stringstream ss(s);
        ss >> x;
        return x;
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
               const std::vector<std::string> &rowNames,
               const std::vector<std::vector<T>> &mat,
               const std::string &topLeft)
{
    putchar('\n');
    putchar('\n');

    begin("tabular");
    std::cout << "{ | c || c ";
    for (size_t i = 1; i < columnNames.size(); i++)
        std::cout << " | c";
    std::cout << " | } \n";

    std::cout << "\\hline\n";
    // Draws the column names
    std::cout << topLeft << " & ";
    DrawVector(columnNames, " & ");
    std::cout << "\\\\\n \\hline\n \\hline \n";

    for (int i = 0; i < mat.size(); i++)
    {
        if (i < rowNames.size())
            std::cout << rowNames[i] << " & ";
        else
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
        if (w != e.w)
            return w > e.w;
        else if (u != e.u)
            return u < e.u;
        else
            return v < e.v;
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

    std::vector<Edge<T>>& operator[](const T &u)
    {
        return g[u];
    }

    std::vector<Edge<T>> operator[](const T &u) const
    {
        return g[u];
    }

    void AddEdgeDir(const T &u, const T &v, int w)
    {
        edges.emplace_back(u, v, w);
        g[u].emplace_back(u, v, w);
        vertices.insert(u);
        vertices.insert(v);
    }

    void AddEdge(const T &u, const T &v, int w)
    {
        AddEdgeDir(u, v, w);
        AddEdgeDir(v, u, w);
    }

    std::vector<Edge<T>> GetEdges(bool filter = false) const
    {
        if (!filter)
            return edges;
        else
        {
            std::vector<Edge<T>> v;
            for (int i = 0; i < edges.size(); i += 2)
                v.push_back(edges[i]);
            return v;
        }
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
void DrawVertices(const std::set<T> &s)
{
    std::cout << "\\{ ";
    DrawVector(std::vector<T>(s.begin(), s.end()), ", ");
    std::cout << "\\}";
}

template<class T>
void DrawEdges(const std::vector<Edge<T>> &s, const std::string &name = "")
{
    if (name != "")
        std::cout << name << " = ";
    std::cout << " \\{ ";
    std::cout << s[0].ToString();
    for (size_t i = 1; i < s.size(); i++)
        std::cout << ", " << s[i].ToString();
    std::cout << " \\}";
}

template<class T>
class DijkstraSolver
{
private:
    std::map<T, int> d;
    std::set<std::pair<int, T>> s;

    std::vector<Edge<T>> e;

    Graph<T> g;
    std::vector<std::vector<T>> mat;
    std::vector<std::string> rowNames;

public:
    DijkstraSolver(const Graph<T> &_g, T start, bool ex) : g(_g)
    {
        mat.push_back(std::vector<std::string>());
        for (int i = 0; i < g.V(); i++)
            mat.back().push_back("\\infty");
        auto v = g.GetVertices();
        s.insert({ 0, start });

        std::map<T, int> d;
        for (auto& x : v)
            d[x] = std::numeric_limits<int>::max();
        d[start] = 0;

        rowNames.push_back("");

        while (!s.empty())
        {
            auto help = s.begin()->second;
            s.erase(s.begin());

            rowNames.push_back("Trenutni cvor: " + help);
            
            mat.push_back(std::vector<std::string>());

            for (auto& next : g[help])
            {
                if (d[next.v] > d[help] + next.w)
                {
                    s.erase({ d[next.v], next.v });
                    d[next.v] = next.w + d[help];
                    s.insert({ next.w + d[help], next.v });
                }
            }

            for (auto& x : v)
                if (d[x] != std::numeric_limits<int>::max())
                    mat.back().push_back(patch::ToString(d[x]));
                else
                    mat.back().push_back("\\infty");
        }
    }

    std::map<T, int> GetD() const
    {
        return d;
    }

    DijkstraSolver<T> Draw()
    {
        auto v = g.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                     rowNames,
                     mat,
                     "Iteracija");
        std::cout << "\n\n";
        return (*this);
    }
};

int main()
{
    freopen("treci.txt", "r", stdin);
    int u, v, w;
    Graph<std::string> g;
    
    std::vector<std::string> vnames;

    while (isalpha(std::cin.peek()))
    {
        std::string name;
        std::cin >> name;
        vnames.push_back(name);
        std::getchar();
    }

    vnames.pop_back();

    DrawVector(vnames, ", "); std::cout << '\n';

    for (int i = 0; i < vnames.size(); i++)
    {
        std::string buff;
        std::cin >> buff;
        if (buff != "-")
            g.AddEdgeDir(vnames[0], vnames[i], patch::ToInt(buff));
    }

    for (int i = 1; i < vnames.size(); i++)
    {
        std::string buff;
        std::cin >> buff;
        for (auto& y : vnames)
        {
            std::cin >> buff;
            if (buff != "-")
                g.AddEdgeDir(vnames[i], y, patch::ToInt(buff));
        }
    }
    
    (DijkstraSolver<std::string>(g, vnames[0], false)).Draw();

    return 0;
}


































