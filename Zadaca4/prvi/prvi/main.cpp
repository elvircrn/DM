#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
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
    if (v.empty())
        return;
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

class Bitmask
{
    long long mask;
public:

    Bitmask()
    {
        mask = 0;
    }
    Bitmask(long long _mask)
    {
        mask = _mask;
    }

    long long get()
    {
        return mask;
    }

    void turn_on(long long index)
    {
        if (!(mask & (1<<index)))
            mask += (1<<index);
    }

    void turn_off(long long index)
    {
        if (mask & (1<<index))
            mask -= (1<<index);
    }

    bool bit_at(long long index)
    {
        return (mask & (1<<index) > 0);
    }

    int count()
    {
        int ret = 0;
        long long m = mask;
        while (m)
        {
            ret += (m & 1);
            m >>= 1;
        }
        return ret;
    }

    std::vector<int> getBits()
    {
        int index = 0;
        long long m = mask;
        std::vector<int> ret;
        while (m)
        {
            if (m % 2)
                ret.push_back(index);
            m >>= 1;
            index++;
        }
        return ret;
    }
};

template<class T>
class Graph
{
    std::map<T, std::vector<Edge<T>>> g;
    std::set<T> vertices;
    std::vector<Edge<T>> edges;
    std::map<T, int> indeg, outdeg;
    std::map<T, std::map<T, int>> m;
    std::map<T, std::map<T, int>> flow;
public:

    void Clear()
    {
        g.clear();
        vertices.clear();
        edges.clear();
        m.clear();
        flow.clear();
        outdeg.clear();
        indeg.clear();
    }

    std::pair<std::vector<T>, std::vector<T>> GetK33()
    {
        int ind = 0;

        std::map<T, int> enc;
        std::vector<T> dec(vertices.size());

        for (auto& v: vertices)
        {
            enc[v] = ind++;
            dec[ind - 1] = v;
        }

        int vsize = vertices.size();

        for (int i = 0; i < (1<<vsize); i++)
        {
            Bitmask one(i);
            if (one.count() != 3)
                continue;
            for (int j = 0; j < (1<<vsize); j++)
            {
                Bitmask two(j);

                if (two.count() != 3 || ((one.get() & two.get()) != 0))
                    continue;

                bool valid = true;
                for (auto& x : one.getBits())
                {
                    for (auto& y : two.getBits())
                    {
                        if (x != y && !m[dec[x]][dec[y]])
                        {
                            valid = false;
                            break;
                        }
                    }
                    if (!valid)
                        break;
                }

                if (valid)
                {
                    std::vector<T> f, s;
                    for (auto& x : one.getBits())
                        f.push_back(dec[x]);
                    for (auto& x : two.getBits())
                        s.push_back(dec[x]);

                    return { f, s };
                }
            }
        }

        return { std::vector<T>(), std::vector<T>() };
    }

    std::vector<T> GetK5()
    {
        int ind = 0;
        std::map<T, int> enc;
        std::vector<T> dec(vertices.size());

        for (auto& v: vertices)
        {
            enc[v] = ind++;
            dec[ind - 1] = v;
        }

        for (int i = 0; i < (1<<vertices.size()); i++)
        {
            Bitmask mask(i);
            if (mask.count() != 5)
                continue;
            bool valid = true;
            for (auto& x : mask.getBits())
            {
                for (auto& y : mask.getBits())
                {
                    if (x != y && !m[dec[x]][dec[y]])
                    {
                        valid = false;
                        break;
                    }
                }
                if (!valid)
                    break;
            }

            if (valid)
            {
                std::vector<T> ret;
                for (auto& x : mask.getBits())
                    ret.push_back(dec[x]);
                return ret;
            }
        }

        return std::vector<T>();
    }

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
        m[u][v] = w;
        outdeg[u]++;
        indeg[v]++;
        edges.emplace_back(u, v, w);
        g[u].emplace_back(u, v, w);
        vertices.insert(u);
        vertices.insert(v);
    }

    std::map<T, std::map<T, int>> GetMatrix()
    {
        return m;
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

    std::map<T, int> GetIndegs()
    {
        return indeg;
    }

    int Indeg(const T& x)
    {
        return indeg[x];
    }

    int Outdeg(const T& x)
    {
        return outdeg[x];
    }

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

    void DrawAdj(const std::string &name, bool drawWeights = false)
    {
        std::cout << "Lista susjedstva za graf " << name << '\n';
        begin("equation*");
        putchar('\n');
        begin("aligned");
        putchar('\n');
        for (auto& u : vertices)
        {
            std::cout << u << " &\\rightarrow ";
            std::cout << " \\{ " << g[u][0].v;

            for (int i = 1; i < g[u].size(); i++)
            {
                auto e = g[u][i];
                std::cout << ", " << e.v;
            }
            std::cout << " \\}\\\\\n";
        }
        end("aligned");
        putchar('\n');
        end("equation*");
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

    void RemoveVertex(const T& vertex)
    {
        vertices.erase(vertex);
        g[vertex].clear();
        outdeg[vertex] = 0;
        indeg[vertex] = 0;

        for (auto& v : vertices)
        {
            m[vertex][v] = 0;
            m[v][vertex] = 0;
        }
    }

    Graph<T> Reduce() const
    {
        Graph<T> ret = (*this);
        for (auto& v : ret.vertices)
            if (ret.outdeg[v] == 2)
            {
                ret.RemoveVertex(v);
                ret.AddEdge(ret.g[v][0].v, ret.g[v][1].v, 1);
            }
        return ret;
    }

    std::vector<int> GetIndegSorted()
    {
        std::vector<int> ret;
        for (auto& v : vertices)
            ret.push_back(indeg[v]);
        std::sort(ret.begin(), ret.end());
        return ret;
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

int main()
{
    freopen("prvi.txt", "r", stdin);

    char u[4], v[4];
    Graph<std::string> g;
    std::map<std::string, int> indeg, outdeg;

    int index = 0;

    while (getchar() == '{')
    {
        g.Clear();
        std::string gname = "\\(G_" + patch::ToString(++index) + "\\)";
        for (;;)
        {
            getchar();
            u[0] = getchar();
            u[1] = getchar();
            u[2] = '\0';
            getchar();
            getchar();
            v[0] = getchar();
            v[1] = getchar();
            v[2] = '\0';
            getchar();
            g.AddEdge(patch::ToString(u), patch::ToString(v), 1);

            if (std::cin.peek() == ',')
            {
                getchar();
                getchar();
            }
            else if (std::cin.peek() == '}')
            {
                getchar();
                getchar();
                break;
            }
        }

        /* a */
        /*
                g.DrawAdj(gname);
                g.DrawGraphTable();
                putchar('\n');
        */
        /* c */
        /*
        test planarnosti
        if (g.Reduce().GetK33().first.size() > 0)
        {
            std::cout << "K33 grafa " << gname << " glasi:\n";
            std::cout << '\t';
            DrawVector(g.Reduce().GetK33().first, ",");
            std::cout << '\n';
            std::cout << '\t';
            DrawVector(g.Reduce().GetK33().second, ",");
            std::cout << '\n';

            g.Reduce().DrawGraphTable();
        }
        if (g.Reduce().GetK5().size() > 0)
        {
            std::cout << "K5 grafa " << gname << " glasi:\n";
            DrawVector(g.Reduce().GetK5(), ",");
        }*/

        DrawVector(g.GetIndegSorted(), ", ");
        std::cout  << '\n';
    }

    return 0;
}
































