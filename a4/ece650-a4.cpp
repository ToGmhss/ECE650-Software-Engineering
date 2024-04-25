// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <queue>
#include <stack>
#include <climits>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

using namespace std;

struct Edge
{
    int destination;
    int weight;
};

class Graph
{
public:
    int vertices;
    // unordered_map<int, vector<int>> adjList;
    vector<vector<Edge>> adjacencyList;
    vector<vector<int>> adjMatrix;
    // int **adjMatrix;
    Graph(int V = 0) : vertices(V), adjacencyList(V + 1), adjMatrix(V + 1, vector<int>(V + 1, 0)) {}
    // Graph(int V = 0) : vertices(V), adjacencyList(V + 1){
    //     adjMatrix = new int*[V + 1];
    //     for (int i = 0; i <= V; ++i) {
    //         adjMatrix[i] = new int[V + 1];
    //         for (int j = 0; j <= V; ++j) {
    //             adjMatrix[i][j] = 0;
    //         }
    //     }
    // }

    // ~Graph() {
    //     for (int i = 0; i <= vertices; ++i) {
    //         delete[] adjMatrix[i];
    //     }
    //     delete[] adjMatrix;
    // }

    bool isEmpty()
    {
        if (vertices == 0)
        {
            return true;
        }
        return false;
    }

    bool isAdjacencyListEmpty() const
    {
        for (const auto &edges : adjacencyList)
        {
            if (!edges.empty())
            {
                return false;
            }
        }
        return true;
    }

    // modified
    void addEdge(int source, int destination)
    {
        if (source <= 0 || source > vertices || destination <= 0 || destination > vertices)
        {
            cerr << "Error: addEdge: Invalid vertex index." << endl;
            return;
        }
        if (source == destination)
        {
            cerr << "Error: Self loop detected" << endl;
            return;
        }
        Edge edgeToDestination;
        edgeToDestination.destination = destination;
        edgeToDestination.weight = 1;
        adjacencyList[source].push_back(edgeToDestination);

        Edge edgeToSource;
        edgeToSource.destination = source;
        edgeToSource.weight = 1;
        adjacencyList[destination].push_back(edgeToSource);
        // std::cout << "source, des " << source << " " << destination << " " << std::endl;
        // std::cout << adjMatrix.size() << " " << adjMatrix[0].size() << std::endl;
        adjMatrix[source][destination] = 1;
        adjMatrix[destination][source] = 1;
    }

    vector<int> dijkstra(int source, vector<int> &predecessor)
    {
        vector<int> distance(vertices + 1, INT_MAX);
        distance[source] = 0;
        predecessor.resize(vertices + 1, -1);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, source});

        while (!pq.empty())
        {
            int u = pq.top().second;
            pq.pop();

            for (const Edge &edge : adjacencyList[u])
            {
                int v = edge.destination;
                int w = edge.weight;
                // relax when necessary
                if (distance[v] > distance[u] + w)
                {
                    distance[v] = distance[u] + w;
                    predecessor[v] = u;
                    pq.push({distance[v], v});
                }
            }
        }

        return distance;
    }

    stack<int> getPath(int source, int target, const vector<int> &predecessor)
    {
        stack<int> path;
        if (predecessor[target] == -1)
        {
            return path;
        }
        for (int current = target; current != source; current = predecessor[current])
        {
            path.push(current);
        }
        path.push(source);
        return path;
    }

    bool verticeInBound(int n)
    {
        if (1 <= n && n <= vertices)
        {
            return true;
        }
        return false;
    }

    void clearEdges()
    {
        for (auto &adjListForVertex : adjacencyList)
        {
            adjListForVertex.clear();
        }
    }

    bool edgeExists(int source, int destination)
    { // no need to check both ways
        if (source <= 0 || source > vertices)
        {
            cerr << "Invalid source vertex.\n";
            return false;
        }
        if (destination <= 0 || destination > vertices)
        {
            cerr << "Invalid destination vertex.\n";
            return false;
        }
        for (const Edge &edge : adjacencyList[source])
        {
            if (edge.destination == destination)
            {
                return true;
            }
        }
        return false;
    }

    vector<int> reduce(int k)
    {
        std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
        std::vector<std::vector<Minisat::Lit>> matrix(vertices);
        // initilization for matrix
        for (int n = 0; n < vertices; n++)
        {
            for (int i = 0; i < k; i++)
            {
                Minisat::Lit lit_var = Minisat::mkLit(solver->newVar());
                matrix[n].push_back(lit_var);
            }
        }
        // 1. At least one vertex is the ith vertex in the vertex cover
        for (int i = 0; i < k; i++)
        {
            Minisat::vec<Minisat::Lit> lit_vec;
            for (int x = 0; x < vertices; x++)
            {
                lit_vec.push(matrix[x][i]);
            }
            solver->addClause(lit_vec);
            lit_vec.clear(); // not clear ......
        }
        // 2. No one vertex can appear twice in a vertex cover.
        for (int m = 0; m < vertices; m++)
        {
            for (int p = 0; p < k - 1; p++)
            {
                for (int q = p + 1; q < k; q++)
                {
                    solver->addClause(~matrix[m][p], ~matrix[m][q]);
                }
            }
        }
        // 3. No more than one vertex appears in the mth position of the vertex cover
        for (int m = 0; m < k; m++)
        {
            for (int p = 0; p < vertices - 1; p++)
            {
                for (int q = p + 1; q < vertices; q++)
                {
                    solver->addClause(~matrix[p][m], ~matrix[q][m]);
                }
            }
        }
        // 4.Every edge is incident to at least one vertex in the vertex cover
        for (int i = 1; i <= vertices; i++)
        {
            for (int j = 1; j <= vertices; j++)
            {
                if (adjMatrix[i][j] != 1)
                {
                    continue;
                }
                Minisat::vec<Minisat::Lit> lit_vec;
                for (int x = 0; x < k; x++)
                {
                    lit_vec.push(matrix[i - 1][x]);
                    lit_vec.push(matrix[j - 1][x]);
                }
                solver->addClause(lit_vec);
                lit_vec.clear();
            }
        }

        // get res
        bool res = solver->solve();
        if (res)
        {
            std::vector<int> result;
            for (int i = 0; i < vertices; i++)
            {
                for (int j = 0; j < k; j++)
                {
                    if (Minisat::toInt(solver->modelValue(matrix[i][j])) == 0)
                    {
                        result.push_back(i + 1);
                    }
                }
            }
            // sort
            solver.reset(new Minisat::Solver());
            std::sort(result.begin(), result.end());
            return result;
        }
        else
        {
            solver.reset(new Minisat::Solver());
            return {-1};
        }
    }

    // for debug
    // void printEdges() {
    //     for (const auto& pair : adjList) {
    //         int fromVertex = pair.first;
    //         const vector<int>& neighbors = pair.second;
    //         for (int toVertex : neighbors) {
    //             cout << fromVertex << " -> " << toVertex << endl;
    //         }
    //     }
    // }
};

/*
Todo:
As soon as you get an input graph via the ’V’ and ’E’ specification you should compute a
minimum-sized Vertex Cover, and immediately output it. The output should just be a sequence of
vertices in increasing order separated by one space each. You can use qsort(3) or std::sort for
sorting.
*/


int main(int argc, char **argv)
{
    Graph g;

    // deals with input
    while (!cin.eof())
    {
        string line;
        getline(cin, line);

        char command = line[0];
        // New V forgets prev info
        if (command == 'V')
        {
            g = Graph(0); // reset g
            int num;
            if (line.length() <= 2)
            {
                cerr << "Error: Incomplete command for V" << endl;
                continue;
            }
            stringstream ss(line.substr(2));
            ss >> num;
            if (num > 1)
            {
                g = Graph(num);
                // g.setVertices(num);
            }
            else
            {
                cerr << "Error: Number of vertices must be greater than 1" << endl;
                continue;
            }
        }
        // Invalid E resets the graph
        else if (command == 'E')
        {
            // Err1: No prev V, that is: graph is empty
            if (g.isEmpty())
            {
                cerr << "Error: Graph has no vertices!" << endl;
                continue;
            }
            // g.clearEdges();
            //  Err2: After valid E
            if (!g.isAdjacencyListEmpty())
            {
                cerr << "Error: Redundant E command, graph already valid." << endl;
                continue;
            }
            regex pattern("<(\\d+),(\\d+)>");
            auto begin = sregex_iterator(line.begin(), line.end(), pattern);
            // bool exitLoop = false;
            for (sregex_iterator i = begin; i != sregex_iterator(); ++i)
            {
                smatch match = *i;
                int u = stoi(match[1].str()), v = stoi(match[2].str());
                // Err3: vertice our of bound
                bool firstInBound = g.verticeInBound(u);
                bool secondInBound = g.verticeInBound(v);
                if ((not firstInBound) || (not secondInBound))
                {
                    // reset the graph
                    g = Graph(0); // reset g
                    cerr << "Error: Vertex doesn't exist! Graph will reset." << endl;
                    // exitLoop = true;
                    break;
                }
                // Err4: adding the same edge multiple times
                if (g.edgeExists(u, v))
                {
                    g = Graph(0); // reset g
                    cerr << "Error: Edge already exists! Graph will reset." << endl;
                    // exitLoop = true;
                    break;
                }
                // Err5: self loop
                if (u == v)
                {
                    g = Graph(0); // reset g
                    cerr << "Error: Self loop detected" << endl;
                    break;
                }
                g.addEdge(u, v);
            }
            // if (exitLoop) {
            //     continue;
            // }
            /*
            --------------------------DELETE before submit!!!!----------------------
            */
            // g.printEdges();

            /*
            -------------------------Here begins a4 core-------------------------
            first get smallest possible k, then get the answer
            */

            vector<int> res;
            vector<int> no_res = {-1};
            for (int k = 1; k <= g.vertices; k++)
            {
                res = g.reduce(k);
                // bool sat = std::equal(res.begin(), res.end(), no_res.begin());
                // if (not sat) {
                //     break;
                // }
                if (res != no_res) {
                    break; // 如果res和no_res不完全相同，则跳出循环
                }
            }
            for (int i = 0; i < res.size(); i++)
            {
                cout << res[i] << " ";
            }
            cout << endl;
        }
        // Only one output!!!
    }

    return 0;
}
