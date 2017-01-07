#include <iostream>
#include <cstdio>
#include <queue>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <map>
#include <set>
#include <stack>
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
class Graph
{
    std::map<T, std::vector<Edge<T>>> g;
    std::set<T> vertices;
    std::vector<Edge<T>> edges;
    std::map<T, int> indeg, outdeg;
public:

    void InvertWeights()
    {
        for (auto& e : edges)
            e.w *= -1;
        for (auto &v : vertices)
            for (auto &x : g[v])
                x.w *= -1;
    }

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
        outdeg[u]++;
        indeg[v]++;
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

    std::map<T, int> GetIndegs() { return indeg; }

    int Indeg(const T& x) { return indeg[x]; }

    int Outdeg(const T& x) { return outdeg[x]; }

    int E() const
    {
        return (int)edges.size();
    }

    void DrawGraph(const std::string &name) const
    {
        std::cout << name << "_V = ";
        DrawVertices(vertices);
        std::cout << '\n';
        std::cout << name << "_E = ";
        DrawEdges(edges);
    }

    void DrawGraphTable() const
    {
        std::map<T, std::map<T, int>> dist;
        std::vector<std::vector<std::string>> mat;
        for (auto& x : vertices)
            for (auto& y : vertices)
                dist[x][y] = std::numeric_limits<int>::max();
        for (auto& e : edges)
            dist[e.u][e.v] = e.w;

        for (auto& x : vertices)
        {
            mat.push_back(std::vector<std::string>());
            for (auto& y : vertices)
            {
                mat.back().push_back(
                    (dist[x][y] == std::numeric_limits<int>::max()) ? "-" :
                                    patch::ToString(dist[x][y]));
            }
        }

        DrawTable<T>(
                std::vector<T>(vertices.begin(), vertices.end()),
                std::vector<T>(vertices.begin(), vertices.end()),
                mat,
                ""
                );
    }
};


template<class T>
class DegreeSolver
{
private:
    std::map<T, int> d;
    std::set<std::pair<int, T>> s;

    std::vector<Edge<T>> e;

    Graph<T> g;
    std::vector<std::string> indeg;
    std::vector<std::string> outdeg;
    std::vector<std::string> rowNames;
    std::vector<std::vector<T>> parentmat;

public:
    DegreeSolver(const Graph<T> &_g) : g(_g)
    {
        auto vertices = g.GetVertices();
        for (auto& v : vertices)
            indeg.push_back(patch::ToString(g.Indeg(v)));
        for (auto& v : vertices)
            outdeg.push_back(patch::ToString(g.Outdeg(v)));
    }

    DegreeSolver<T> Draw(const std::string &topLeft)
    {
        auto v = g.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                { "Indeg: " },
                { indeg },
                topLeft);
        std::cout << '\n';
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                { "Outdeg" },
                { outdeg },
                topLeft);
        std::cout << "\n\n";
        return (*this);
    }
};

template<class T>
class DFSCycleFinder
{
private:
    std::map<T, int> d;
    std::set<std::pair<int, T>> s;

    std::vector<Edge<T>> e;

    Graph<T> g;
    std::vector<std::vector<std::string>> mat;
    std::vector<std::string> rowNames;
    std::vector<std::vector<T>> parentmat;
    std::map<T, int> color;
    std::set<T> vertices;

    const int DFS_WHITE = 0;
    const int DFS_GREY  = 1;
    const int DFS_BLACK = 2;

    std::vector<std::string> colorNames;

public:
    void Dfs(const T &node)
    {
        if (color[node] == DFS_BLACK)
            return;
        color[node] = DFS_GREY;
        mat.push_back(std::vector<std::string>());
        rowNames.push_back("Cvor:" + patch::ToString<T>(node));
        for (auto& v : vertices)
            mat.back().push_back(colorNames[color[v]]);
        for (auto& e : g[node])
            if (color [e.v] == DFS_GREY)
                std::cout << "Postoji ciklus!\n";
            else if (color[e.v] == DFS_WHITE)
                Dfs(e.v);
        color[node] = DFS_BLACK;
    }

    DFSCycleFinder(const Graph<T> &_g) : g(_g), colorNames({ "W", "G", "B" } )
    {
        vertices = g.GetVertices();
        rowNames.push_back("Inicijalno: ");
        mat.push_back(std::vector<std::string>());
        for (auto& v : vertices)
            mat.back().push_back(colorNames[color[v]]);
        for (auto& v : vertices)
            Dfs(v);
        rowNames.push_back("Konacno: ");
        mat.push_back(std::vector<std::string>());
        for (auto& v : vertices)
            mat.back().push_back(colorNames[color[v]]);
    }

    DFSCycleFinder<T> Draw(const std::string &topLeft)
    {
        auto v = g.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                rowNames,
                mat,
                topLeft);
        std::cout << "\n\n";
        return (*this);
    }
};

template<class T>
class TopSortSolver
{
private:
    std::map<T, int> d;

    std::vector<Edge<T>> e;

    Graph<T> g;
    std::vector<std::vector<std::string>> mat;
    std::vector<std::string> rowNames;
    std::vector<std::vector<T>> parentmat;
    std::map<T, int> color;
    std::set<T> vertices;
    std::vector<std::string> colorNames;
    std::map<T, bool> visited;

    std::stack<T> s;

    void Dfs(const T& node)
    {
        visited[node] = true;
        for (auto& next : g[node])
            if (!visited[next.v])
                Dfs(next.v);
        s.push(node);
    }

public:

    TopSortSolver(const Graph<T> &_g) : g(_g)
    {
        vertices = g.GetVertices();

        for (auto& v : vertices)
            if (!visited[v])
                Dfs(v);

        std::vector<T> topsort;

        while (!s.empty())
        {
            topsort.push_back(s.top());
            s.pop();
        }

        std::cout << "Topolosko sortiranje: ";
        DrawVector(topsort, ", ");
    }

    TopSortSolver<T> Draw(const std::string &topLeft)
    {
        auto v = g.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                rowNames,
                mat,
                topLeft);
        std::cout << "\n\n";
        return (*this);
    }
};


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
    std::map<T, T> parent;
    std::vector<T> path;

public:
    DijkstraSolver(const Graph<T> &_g,
                   const T& start,
                   const T& end) : g(_g)
    {
        mat.push_back(std::vector<std::string>());
        for (int i = 0; i < g.V(); i++)
            mat.back().push_back("\\infty");
        auto v = g.GetVertices();
        s.insert({ 0, start });

        std::map<T, int> d;
        for (auto& x : v)
        {
            d[x] = std::numeric_limits<int>::max();
            parent[x] = x;
        }
        d[start] = 0;

        rowNames.push_back("");

        while (!s.empty())
        {
            auto help = s.begin()->second;
            s.erase(s.begin());

            rowNames.push_back("Trenutni cvor: " + help);

            if (help == end)
                break;

            mat.push_back(std::vector<std::string>());

            for (auto& next : g[help])
            {
                if (d[next.v] > d[help] + next.w)
                {
                    s.erase({ d[next.v], next.v });
                    d[next.v] = next.w + d[help];
                    s.insert({ next.w + d[help], next.v });
                    parent[next.v] = help;
                }
            }

            for (auto& x : v)
                if (d[x] != std::numeric_limits<int>::max())
                    mat.back().push_back(patch::ToString(d[x]));
                else
                    mat.back().push_back("\\infty");
        }

        T node = end;

        while (parent[node] != node)
        {
            path.push_back(node);
            node = parent[node];
        }
        path.push_back(node);

        std::reverse(path.begin(), path.end());

        std::cout << "Najkraci put od " << start << " do " << end << ": ";
        DrawVector(path, " \\rightarrow ");
        std::cout << "\\\\\n";
        std::cout << " i tezak je " << d[end] << " imaginarnih jedinica.";
    }

    DijkstraSolver(const Graph<T> &_g) : g(_g)
    {
        auto v = g.GetVertices();
        auto e = g.GetEdges();

        std::map<T, std::map<T, int>> d;

        for (auto& x : v)
            for (auto& y : v)
                d[x][y] = std::numeric_limits<int>::max();

        for (auto& x : v)
        {
            d[x][x] = 0;
        }

        for (auto& start : v)
        {
            s.insert({ 0, start });
            while (!s.empty())
            {
                auto help = s.begin()->second;
                s.erase(s.begin());

                for (auto& next : g[help])
                {
                    if (d[start][next.v] > d[start][help] + next.w)
                    {
                        s.erase({ d[start][next.v], next.v });
                        d[start][next.v] = next.w + d[start][help];
                        d[next.v][start] = d[start][next.v];
                        s.insert({ next.w + d[start][help], next.v });
                    }
                }
            }
        }

        for (auto& x : v)
        {
            mat.push_back(std::vector<T>());
            rowNames.push_back(x);
            for (auto& y : v)
                mat.back().push_back(patch::ToString(d[x][y]));
        }
    }


    std::map<T, int> GetD() const
    {
        return d;
    }

    DijkstraSolver<T> Draw(const std::string &topLeft)
    {
        auto v = g.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                rowNames,
                mat,
                topLeft);
        std::cout << "\n\n";
        return (*this);
    }
};


template<class T>
class BellmanFordSolver 
{
private:
    std::map<T, int> d;
    std::set<std::pair<int, T>> s;

    std::vector<Edge<T>> e;

    Graph<T> g;
    std::vector<std::vector<T>> mat;
    std::vector<std::string> rowNames;
    std::vector<std::vector<T>> parentmat;

public:
    BellmanFordSolver(const Graph<T> &_g, const T &start, const T &end, bool rev = false) : g(_g)
    {
        int INF;
        INF = std::numeric_limits<int>::max() / 2;

        if (rev)
        {
            g.InvertWeights();
            g.DrawGraphTable();
        }

        auto v = g.GetVertices();
        auto e = g.GetEdges();

        std::map<T, int> d;
        std::map<T, T> parent;

        for (auto& x : v)
            d[x] = INF;

        d[start] = 0;
        parent[start] = start;

        std::sort(e.begin(), e.end(),
                [](const Edge<T> &a, const Edge<T> &b) -> bool { return a.w < b.w; });

        rowNames.push_back("Inicijalno stanje: ");

        mat.push_back(std::vector<T>());
        parentmat.push_back(std::vector<T>());

        for (auto& x : v)
        {
            if (d[x] != INF)
                mat.back().push_back(patch::ToString(d[x]));
            else
                mat.back().push_back("\\(\\infty\\)");
            parentmat.back().push_back(x);
        }

        int it = 0;
        bool relax = true;

        while (relax)
        {
            relax = false;
            rowNames.push_back("Iteracija " + patch::ToString((it++) + 1));
            for (auto& edge : e)
            {
                if (d[edge.v] > d[edge.u] + edge.w)
                {
                    relax = true;
                    d[edge.v] = d[edge.u] + edge.w;
                    parent[edge.v] = edge.u;
                }
            }

            mat.push_back(std::vector<T>());
            parentmat.push_back(std::vector<T>());
            for (auto& x : v)
            {
                if (d[x] != INF)
                    mat.back().push_back(patch::ToString(d[x]));
                else
                    mat.back().push_back("\\(\\infty\\)");
                parentmat.back().push_back(parent[x]);
            }
        }

        std::vector<std::string> path;
        T node = end;

        while (parent[node] != node)
        {
            path.push_back(node);
            node = parent[node];
        }

        path.push_back(node);

        std::reverse(path.begin(), path.end());

        std::cout << "Put: ";

        DrawVector(path, " \\rightarrow ");
    }

    BellmanFordSolver<T> Draw(const std::string &topLeft)
    {
        auto v = g.GetVertices();
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                rowNames,
                mat,
                topLeft);
        std::cout << '\n';
        DrawTable<T>(std::vector<T>(v.begin(), v.end()),
                rowNames,
                parentmat,
                "");
        std::cout << "\n\n";
        return (*this);
    }
};

int main()
{
    freopen("peti.txt", "r", stdin);
    int w;
    int u, v;
    Graph<std::string> g;
    while (scanf("(x%d, x%d, %d)", &u, &v, &w) == 3)
    {
        std::cout << u << ' ' << v << ' ' << w << '\n';
        g.AddEdgeDir("\\(x_{" + patch::ToString(u) + "}\\)",
                     "\\(x_{" + patch::ToString(v) + "}\\)",
                     w);
        getchar();
        getchar();
    }

    g.DrawGraph("G");
    std::cout << '\n';
    g.DrawGraphTable();
    std::cout << "\n\n\n";
    //(DFSCycleFinder<std::string>(g)).Draw("");
    //(TopSortSolver<std::string>(g));

    std::string start, end;

    for (auto& x : g.GetVertices())
    {
        if (!g.Indeg(x))
            start = x;
        if (!g.Outdeg(x))
            end = x;
    }

    //(DijkstraSolver<std::string>(g, start, end)).Draw("");
    //BellmanFordSolver<std::string>(g, start, end).Draw("");
    BellmanFordSolver<std::string>(g, start, end, true).Draw("");

    return 0;
}































