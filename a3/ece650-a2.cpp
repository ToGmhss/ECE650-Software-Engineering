// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <queue>
#include <stack>
#include <climits>

using namespace std;

struct Edge {
    int destination;
    int weight;
};

class Graph {
public:
    int vertices;
    //unordered_map<int, vector<int>> adjList;
    vector<vector<Edge>> adjacencyList;
    Graph(int V = 0) : vertices(V), adjacencyList(V+1) {}

    bool isEmpty() {
        if (vertices == 0) {
            return true;
        }
        return false;
    }

    bool isAdjacencyListEmpty() const {
        for (const auto& edges : adjacencyList) {
            if (!edges.empty()) {
                return false;
            }
        }
        return true; 
    }

    // new
    void addEdge(int source, int destination) {
        if (source <= 0 || source > vertices || destination <= 0 || destination > vertices) {
            cerr << "Error: addEdge: Invalid vertex index." << endl;
            return;
        }
        if (source == destination) {
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
    }


    vector<int> dijkstra(int source, vector<int>& predecessor) {
        vector<int> distance(vertices+1, INT_MAX);
        distance[source] = 0;
        predecessor.resize(vertices+1, -1);  

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, source});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (const Edge& edge : adjacencyList[u]) {
                int v = edge.destination;
                int w = edge.weight;
                // relax when necessary
                if (distance[v] > distance[u] + w) {
                    distance[v] = distance[u] + w;
                    predecessor[v] = u;  
                    pq.push({distance[v], v});
                }
            }
        }

        return distance;
    }

    stack<int> getPath(int source, int target, const vector<int>& predecessor) {
        stack<int> path;
        if (predecessor[target] == -1) {
            return path;
        }
        for (int current = target; current != source; current = predecessor[current]) {
            path.push(current);
        }
        path.push(source);  
        return path;
    }

    bool verticeInBound(int n) {
        if (1 <= n && n <= vertices) {
            return true;
        }
        return false;
    }


    // not sure --要改
    void clearEdges(){
        for (auto& adjListForVertex : adjacencyList) {
            adjListForVertex.clear(); 
        }
    }

    bool edgeExists(int source, int destination) { // no need to check both ways
        if (source <= 0 || source > vertices) {
            cerr << "Invalid source vertex.\n";
            return false;
        }
        if (destination <= 0 || destination > vertices) {
            cerr << "Invalid destination vertex.\n";
            return false;
        }
        for (const Edge& edge : adjacencyList[source]) {
            if (edge.destination == destination) {
                return true; 
            }
        }
        return false; 
    }
};



/*
Note:
The graph is specified by the specification of the set of vertices V followed by the set of edges E,
in that order. V and E always occur together. There is no relationship between subsequent graph
specifications; when a new V specification starts, all previous information can be forgotten. After
the specification of the graph there can be zero or more shortest-path queries s. For each s query,
only one shortest path should be output; multiple shortest paths might exist and an arbitrary choice
can be made.
*/
int main(int argc, char** argv) {
    Graph g;

    // deals with input
    while (!cin.eof()) {
        string line;
        getline(cin, line);
        
        char command = line[0];
        // New V forgets prev info
        if (command == 'V') {
            g = Graph(0); // reset g
            int num;
            if (line.length() <= 2) {
                cerr << "Error: Incomplete command for V" << endl;
                continue;
            }
            stringstream ss(line.substr(2));
            ss >> num;
            if (num > 1) {
                g = Graph(num);
                //g.setVertices(num);
            } 
            else {
                cerr << "Error: Number of vertices must be greater than 1" << endl;
                continue;
            }
            // modify for a3 requirement;
            cout << line << endl;
        } 
        // Invalid E resets the graph
        else if (command == 'E') {
            // Err1: No prev V, that is: graph is empty
            if (g.isEmpty()) {
                cerr << "Error: Graph has no vertices!" << endl;
                continue;
            }
            //g.clearEdges();
            // Err2: After valid E
            if (!g.isAdjacencyListEmpty()) {
                cerr << "Error: Redundant E command, graph already valid." << endl;
                continue;
            }
            regex pattern("<(\\d+),(\\d+)>");
            auto begin = sregex_iterator(line.begin(), line.end(), pattern);
            //bool exitLoop = false;
            for (sregex_iterator i = begin; i != sregex_iterator(); ++i) {
                smatch match = *i;
                int u = stoi(match[1].str()), v = stoi(match[2].str());
                // Err3: vertice our of bound
                bool firstInBound = g.verticeInBound(u);
                bool secondInBound = g.verticeInBound(v);
                if ((not firstInBound) || (not secondInBound)) {
                    // reset the graph
                    g = Graph(0); // reset g
                    cerr << "Error: Vertex doesn't exist! Graph will reset." << endl;
                    //exitLoop = true;
                    break;
                }
                // Err4: adding the same edge multiple times
                if (g.edgeExists(u, v)) {
                    g = Graph(0); // reset g
                    cerr << "Error: Edge already exists! Graph will reset." << endl;
                    //exitLoop = true;
                    break;
                }
                // Err5: self loop
                if (u == v) {
                    g = Graph(0); // reset g
                    cerr << "Error: Self loop detected" << endl;
                    break;
                }
                g.addEdge(u, v);
            }
            // modify for a3 requirement;
            cout << line << endl;
        } 
        // Only one output!!!
        else if (command == 's') {
            // check for valid g, if not, err
            if (g.isEmpty()) {
                cerr << "Error: graph is empty, no path exists" << endl;
                continue;
            }
            // 需要检查edge吗
            int start, goal;
            if (line.length() <= 2) {
                cerr << "Error: Incomplete command for s" << endl;
                continue;
            }
            stringstream ss(line.substr(2));
            ss >> start >> goal;
            // Err1: vertice our of bound
            bool firstInBound = g.verticeInBound(start);
            bool secondInBound = g.verticeInBound(goal);
            if ((not firstInBound) || (not secondInBound)) {
                cerr << "Error: Vertex doesn't exist!" << endl;
                continue;
            }
            // Err2: if it's self-edge
            if (start == goal) {
                cerr << "Error: Self-loop edge" << endl;
                continue;
            }
            // dij & record pred
            vector<int> predecessor;
            vector<int> shortestDistances = g.dijkstra(start, predecessor);
            stack<int> path = g.getPath(start, goal, predecessor);

            if (path.empty()) {
                cout << "Error: No existing path" << endl;
                continue;
            }
            // output
            //cout << "Shortest path from " << start << " to " << goal << ": ";
            while (!path.empty()) {
                cout << path.top();
                path.pop();
                if (!path.empty()) {
                    cout << "-";
                }
            }
            cout << endl;
            }
        }

    return 0;
}
