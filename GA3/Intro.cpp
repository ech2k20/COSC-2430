#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include "ArgumentManager.h"
using namespace std;

//Paul - 33%, Eric 33%, David 33%

typedef pair<int, int> iPair;

struct DisjointSets
{
    int* parent, * rank;
    int n;

    DisjointSets(int n)
    {
        this->n = n;
        parent = new int[n + 1];
        rank = new int[n + 1];

        // Initially, all vertices are in 
        // different sets and have rank 0. 
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    // Find the parent of a node 'u' 
    // Path Compression 
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u]) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // Union by rank 
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rank[x] > rank[y]) {
            parent[y] = x;
        }
        else {
            parent[x] = y;
        }
        if (rank[x] == rank[y]) {
            rank[y]++;
        }

    }
};

class Graph
{
    int V;    // No. of verticess
    int E;      // No. of edges
    vector<pair<int, int>>* adj;
    vector<pair<int, iPair>> edges; //for #3

public:
    Graph(int V)
    {
        this->V = V;
        E = 0;
        adj = new vector<pair<int, int>>[V];
    }

    void addEdge(int v1, int v2, int w)
    {
        adj[v1].push_back(make_pair(v2, w));
        adj[v2].push_back(make_pair(v1, w));
        edges.push_back({w, {v1, v2}}); // for #3
        E += 1;
    }

    bool BFS(int s) // For #1
    {
        // Mark all the vertices as not visited 
        bool* visited = new bool[V];
        for (int i = 0; i < V; i++) {
            visited[i] = false;
        }

        queue<int> queue;

        // Mark the current node as visited and enqueue it 
        visited[s] = true;
        queue.push(s);

        while (!queue.empty()) {
            // Dequeue a vertex from queue
            s = queue.front();
            //cout << s << " ";
            queue.pop();

            // Get all adjacent vertices of the dequeued 
            // vertex s. If a adjacent has not been visited,  
            // then mark it visited and enqueue it 
            for (auto i = adj[s].begin(); i != adj[s].end(); ++i) {
                if (!visited[i->first]) {
                    visited[i->first] = true;
                    queue.push(i->first);
                }
            }
        }

        for (int i = 0; i < V; i++) {
            if (visited[i] == false) {
                return false;
            }
        }
        return true;
    }

    void DFSUtil(int v, bool visited[]) { // Start of #2
        // Mark the current node as visited and print it 
        visited[v] = true;

        // Recur for all the vertices adjacent to this vertex 
        for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
            if (!visited[i->first]) {
                DFSUtil(i->first, visited);
            }
        }
            
    }

    bool isConnected()
    {
        // Mark all the vertices as not visited 
        bool* visited = new bool[V];
        int i;
        for (i = 0; i < V; i++) {
            visited[i] = false;
        }

        // Find a vertex with non-zero degree 
        for (i = 0; i < V; i++) {
            if (adj[i].size() != 0) {
                break;
            }
        }

        // If there are no edges in the graph, return true 
        if (i == V) {
            return true;
        }

        // Start DFS traversal from a vertex with non-zero degree 
        DFSUtil(i, visited);

        // Check if all non-zero degree vertices are visited 
        for (i = 0; i < V; i++) {
            if (visited[i] == false && adj[i].size() > 0) {
                return false;
            }
        }

        return true;
    }

    
    int isEulerian() // For #2
    {
        /* 0 --> If graph is not Eulerian
        1 --> If graph has an Euler path (Semi-Eulerian)
        2 --> If graph has an Euler Circuit (Eulerian)  */

        // Check if all non-zero degree vertices are connected 
        if (isConnected() == false) {
            return 0;
        }

        // Count vertices with odd degree 
        int oddD = 0;
        for (int i = 0; i < V; i++) {
            if (adj[i].size() & 1) {
                oddD++;
            }
        }

        // If count is more than 2, then graph is not Eulerian 
        if (oddD > 2) {
            return 0;
        }

        return (oddD) ? 1:2;
    }


    int KruskalMST() { //For #3
        
        int e = 0; // An index variable, used for result[]
        int i = 0; // An index variable, used for sorted edges
        int result = 0; // Tnis will store the resultant MST
        DisjointSets DS(V);

        // Sort all the edges in non-decreasing
        // order of their weight. If we are not allowed to
        // change the given graph, we can create a copy of
        // array of edges
        sort(edges.begin(), edges.end());
        for (auto it = edges.begin(); it != edges.end(); it++) {
            int u = it->second.first;
            int v = it->second.second;
            int set_u = DS.find(u);
            int set_v = DS.find(v);

            // Check if the selected edge is creating 
            // a cycle or not (Cycle is created if u 
            // and v belong to same set) 
            if (set_u != set_v) {
                //cout << u << " - " << v << endl; // ----------> Current edge will be in the MST (v1 - v2)

                // Update MST weight 
                result += it->first;

                // Merge two sets 
                DS.merge(set_u, set_v);
            }
        }

        return result; //The MST weight
    }

    bool printLargestAdjVertices(ofstream& output)
    {
        int v2, w, count, numofVertices;
        int size = 0;
        int count2 = 0;
        bool good;

        for (int v = 0; v < V; ++v) //to find the size of the greatest amount of adjacent vertices
        {
            count = 0;
            for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
                count++;
            }
            if (size < count) {
                size = count; 
            }
        }

        for (int v = 0; v < V; ++v) { //to find the number of vertices that had the greatest amount of adjacent vertices
            count = 0;
            for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
                count++;
            }
            if (count == size) {
                count2++;
            }
        }
        numofVertices = count2; 
        count2 = 0;

        for (int v = 0; v < V; ++v)
        {
            count = 0;
            for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
                count++;
            }

            if (count == size) {
                count = 0;
                count2 += 1;
                cout << v << "<-{";
                output << v << "<-{";
                for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
                    v2 = i->first;
                    w = i->second;
                    count++;
                    if (count < size) {
                        cout << v2 << ",";
                        output << v2 << ",";
                    }
                    else {
                        cout << v2;
                        output << v2;
                    }
                    
                }
                if (count2 < numofVertices) {
                    cout << "}, ";
                    output << "}, ";
                }
                else {
                    cout << "}";
                    output << "}";
                }
            }
        }

        cout << endl;
        output << endl;

        if (size >= 3) {
            good = true;
        }
        else {
            good = false;
        }
        
        return good;
    }

    void printGraph()
    {
        int v2, w;
        for (int v = 0; v < V; ++v)
        {
            cout << endl << "Adjacency List of Vertex " << v << endl;
            for (auto i = adj[v].begin(); i != adj[v].end(); ++i) {
                v2 = i->first;
                w = i->second;
                cout << " -> " << v2 << " w/ weight " << w << endl;
            }
            cout << endl;
        }
    }


    void print(ofstream& output, int _sizeV) {

        int numofGood = 0;
        cout << "1. ";
        output << "1. ";
        bool test1 = BFS(0);
        if (test1 == true) {
            cout << "Yes" << endl;
            output << "Yes" << endl;
            numofGood++;
        }
        else {
            cout << "No" << endl;
            output << "No" << endl;
            cout << "Bad";
            output << "Bad";
            return;
        }

        
        /*0 --> If graph is not Eulerian
        1 --> If graph has an Euler path (Semi-Eulerian)
        2 --> If graph has an Euler Circuit (Eulerian)
        Undirected graph so graph cannot be Euler path (Semi-Eulerian)
        Therefore variable test2 cannot have a value of 1*/
        cout << "2. ";
        output << "2. ";
        int test2 = isEulerian();
        if (test2 == 0) {
            cout << "No" << endl;
            output << "No" << endl;
            
        }
        else {
            numofGood++;
            cout << "Yes" << endl;
            output << "Yes" << endl;
        }

        cout << "3. ";
        output << "3. ";
        cout << KruskalMST() << endl;
        output << KruskalMST() << endl;

        cout << "4. ";
        output << "4. ";
        bool test4 = printLargestAdjVertices(output);
        if (test4 == true) {
            numofGood++;
        }

        if (numofGood == 3) {
            cout << "Good";
            output << "Good";
        }
        else {
            cout << "Fair";
            output << "Fair";
        }

    }
};


int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    string infilename = am.get("input");
    string outfilename = am.get("output");

    ifstream infile(infilename);
    ofstream outfile(outfilename);
    
    int sizeV, vertex1, vertex2, weight;
    infile >> sizeV;
    Graph graph(sizeV);

    while (!infile.eof()) {
        infile >> vertex1 >> vertex2 >> weight;
        graph.addEdge(vertex1, vertex2, weight);
    }

    //graph.printGraph(); // this is just for testing purposes for now
    graph.print(outfile, sizeV);

    outfile.close();
    infile.close();

    return 0;
}
