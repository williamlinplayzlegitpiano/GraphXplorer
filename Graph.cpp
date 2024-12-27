#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <limits>
#include <stack>
#include <functional>

using namespace std;

Graph::Graph(const char* const & edgelist_csv_fn) { 
    ifstream my_file(edgelist_csv_fn); // open the CSV file for reading
    if (!my_file.is_open()) return;   // exit if the file cannot be opened

    string line;
    unsigned int nodeIDCount = 0;     // counter for assigning unique IDs to nodes

    while (getline(my_file, line)) {  // read each line of the file
        if (line.empty()) continue;  // skip empty lines

        istringstream ss(line);      // parse the line into components
        string u, v, wstr;
        getline(ss, u, ',');
        getline(ss, v, ',');
        getline(ss, wstr, '\n');
        double w = stod(wstr);       // convert weight to a double

        // add nodes if they don't already exist
        if (imp.nodeMap.find(u) == imp.nodeMap.end()) {
            imp.nodeMap[u] = nodeIDCount++;
            imp.nodeList.push_back(u);
        }
        if (imp.nodeMap.find(v) == imp.nodeMap.end()) {
            imp.nodeMap[v] = nodeIDCount++;
            imp.nodeList.push_back(v);
        }

        unsigned int uID = imp.nodeMap[u];
        unsigned int vID = imp.nodeMap[v];

        // add undirected edge between nodes
        imp.adjList.resize(nodeIDCount);
        imp.adjList[uID].push_back(make_pair(vID, w));
        imp.adjList[vID].push_back(make_pair(uID, w));

        imp.edgeCount++; // increment edge count
    }

    my_file.close(); // close the file
}


// returns the number of nodes in the graph
unsigned int Graph::num_nodes() {
    return static_cast<unsigned int>(imp.nodeList.size());
}

// returns a list of all node labels
vector<string> Graph::nodes() {
    return imp.nodeList;
}

// returns the number of edges in the graph
unsigned int Graph::num_edges() {
    return imp.edgeCount;
}

// returns the number of neighbors for a node
unsigned int Graph::num_neighbors(string const & node_label) {
    unsigned int uID = imp.getNodeID(node_label);
    if (uID == (unsigned int)-1) return 0; //if invalid, no neighbour
    return static_cast<unsigned int>(imp.adjList[uID].size()); //return size of adj list
}

// returns the weight of the edge between two nodes or -1 if no edge exists
double Graph::edge_weight(string const & u_label, string const & v_label) {
    unsigned int uID = imp.getNodeID(u_label);
    unsigned int vID = imp.getNodeID(v_label);
    if (uID == (unsigned int)-1 || vID == (unsigned int)-1) return -1.0; // no edge if invalid IDs
    for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[uID].begin(); iter != imp.adjList[uID].end(); iter++) {
        if (iter->first == vID) return iter->second; // return the weight if edge exists
    }
    return -1.0;// no edge found
}

// returns a list of neighbors for a node
vector<string> Graph::neighbors(string const & node_label) {
    vector<string> result;
    unsigned int uID = imp.getNodeID(node_label);
    if (uID == (unsigned)-1) return result;
    for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[uID].begin(); iter != imp.adjList[uID].end(); iter++) {
        result.push_back(imp.nodeList[iter->first]); // add neighbor label to the result
    }
    return result;
}

//find shortest unweighted path between two nodes
vector<string> Graph::shortest_path_unweighted(string const & start_label, string const & end_label) {
    vector<string> result;
    unsigned int startID = imp.getNodeID(start_label);
    unsigned int endID = imp.getNodeID(end_label);

    if (startID == (unsigned int)-1 || endID == (unsigned int)-1) return result;

    if (startID == endID) {
        result.push_back(start_label); //trivial case where start == end
        return result;
    }

    vector<bool> visitedNodes(num_nodes(), false); //track visited nodes
    vector<int> prevNodes(num_nodes(), -1); //store the previous node in the path
    queue<unsigned int> q; //queue for BFS

    visitedNodes[startID] = true; //mark start as visited node
    q.push(startID);    //enqueue start node

    while (!q.empty()) {
        unsigned int curr = q.front();
        q.pop(); 
        if (curr == endID) break; //stop if end node found
        for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[curr].begin(); iter != imp.adjList[curr].end(); iter++) {
            unsigned int currnbr = iter->first;
            if (!visitedNodes[currnbr]) {
                visitedNodes[currnbr] = true;
                prevNodes[currnbr] = static_cast<int>(curr); //record path
                q.push(currnbr); //enqueue nbr
            }
        }
    }

    if (!visitedNodes[endID]) return result; //no path found

    for (unsigned int curr = endID; curr != (unsigned int)-1; curr = static_cast<unsigned int>(prevNodes[curr])) {
        result.push_back(imp.nodeList[curr]); //reconstruct path
    }
    reverse(result.begin(), result.end()); //reverse for correct order
    return result;
}

vector<tuple<string,string,double>> Graph::shortest_path_weighted(string const & start_label, string const & end_label) {
    vector<tuple<string, string, double>> result;
    unsigned int startID = imp.getNodeID(start_label);
    unsigned int endID = imp.getNodeID(end_label);
    if (startID == (unsigned int)-1 || endID == (unsigned int)-1) return result;

    if (startID == endID) {
        result.push_back(make_tuple(start_label, start_label, -1));
        return result;
    }

    vector<double> nodeDist(num_nodes(), numeric_limits<double>::infinity());
    vector<int> prevNode(num_nodes(), -1);
    nodeDist[startID] = 0.0;

    typedef pair<double, unsigned int> State; // (distance, node_id)
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push(make_pair(0.0, startID));

    while (!pq.empty()) {
        State s = pq.top(); pq.pop();
        double d = s.first;
        unsigned int u = s.second;
        if (d > nodeDist[u]) continue;
        for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[u].begin(); iter != imp.adjList[u].end(); iter++) {
            unsigned int v = iter->first;
            double w = iter->second;
            if (nodeDist[u] + w < nodeDist[v]) {
                nodeDist[v] = nodeDist[u] + w;
                prevNode[v] = static_cast<int>(u);
                pq.push(make_pair(nodeDist[v], v));
            }
        }
    }

    if (nodeDist[endID] == numeric_limits<double>::infinity()) return result; // No path

    vector<unsigned int> pathID;
    for (unsigned int curr = endID; curr != (unsigned int)-1; curr = static_cast<unsigned int>(prevNode[curr])) {
        pathID.push_back(curr);
    }
    reverse(pathID.begin(), pathID.end());

    for (size_t i = 0; i + 1 < pathID.size(); i++) {
        unsigned int u = pathID[i], v = pathID[i + 1];
        double weight = -1.0;
        for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[u].begin(); iter != imp.adjList[u].end(); iter++) {
            if (iter->first == v) {
                weight = iter->second;
                break;
            }
        }
        result.push_back(make_tuple(imp.nodeList[u], imp.nodeList[v], weight));
    }

    return result;
}

vector<vector<string>> Graph::connected_components(double const & threshold) {
    vector<vector<string>> components;
    unsigned int nodeCount = num_nodes();
    vector<bool> visitedNodes(nodeCount, false);

    // for each node, if unvisited, do a BFS or DFS
    for (unsigned int i = 0; i < nodeCount; i++) {
        if (!visitedNodes[i]) {
            // start a new component
            vector<string> component;
            //create stack for DFS
            stack<unsigned int> nodeStack;
            nodeStack.push(i);
            visitedNodes[i] = true;

            while (!nodeStack.empty()) {
                unsigned int u = nodeStack.top(); 
                nodeStack.pop();
                component.push_back(imp.nodeList[u]); // add this node to the current component

                // explore neighbors of u   
                for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[u].begin(); 
                    iter != imp.adjList[u].end(); iter++) {
                    unsigned int v = iter->first;
                    double w = iter->second;
                if (w <= threshold && !visitedNodes[v]) { // only traverse if w <= threshold
                    visitedNodes[v] = true;
                    nodeStack.push(v);
                    }
                }
            }
            components.push_back(component);
        }
    }

    return components;
}

/** 
 *Union-Find data structure for maintaining disjoint sets
*/ 
class UnionFind {
public:
    // Constructor to initialize `n` disjoint sets
    UnionFind(unsigned int n) : parent(n), size(n, 1) {
        // Each node is its own parent initially (self-loop)
        for (unsigned int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Find the root (representative) of the set containing element `a`
    // Uses path compression to flatten the structure for efficiency
    unsigned int findSet(unsigned int a) {
        while (a != parent[a]) {
            parent[a] = parent[parent[a]]; // Path compression: point directly to grandparent
            a = parent[a];
        }
        return a; // Return the root of the set
    }

    // Union the sets containing elements `a` and `b`
    // Uses union by size to keep the tree shallow
    void unionSet(unsigned int a, unsigned int b) {
        // Find the roots of the sets containing `a` and `b`
        a = findSet(a);
        b = findSet(b);

        // If they are already in the same set, do nothing
        if (a != b) {
            // Union by size: attach the smaller tree under the larger tree
            if (size[a] < size[b]) {
                // Swap to ensure `a` is the larger tree
                unsigned int temp = a;
                a = b;
                b = temp;
            }
            parent[b] = a;      // Make `a` the parent of `b`
            size[a] += size[b]; // Update the size of the new root's set
        }
    }

private:
    vector<unsigned int> parent; // Stores the parent of each element
    vector<unsigned int> size;   // Stores the size of the tree rooted at each element
};


// smallest_connecting_threshold(start_label, end_label):
// return smallest threshold such that start and end are connected if we only consider edges <= that threshold
double Graph::smallest_connecting_threshold(string const & start_label, string const & end_label) {
    unsigned int startID = imp.getNodeID(start_label);
    unsigned int endID = imp.getNodeID(end_label);

    if (startID == (unsigned int)-1 || endID == (unsigned int)-1) return -1.0; // invalid nodes
    if (startID == endID) return 0.0; // same node, no edge needed

    // extract all edges (u < v to avoid duplicates)
    vector<tuple<double, unsigned int, unsigned int>> edges;
    for (unsigned int u = 0; u < num_nodes(); u++) {
        for (vector<pair<unsigned int, double>>::iterator iter = imp.adjList[u].begin(); 
        iter != imp.adjList[u].end(); iter++) {
            unsigned int v = iter->first;
            double w = iter->second;
            if (u < v) { // Ensure each edge is only added once (u < v avoids duplicates)
            edges.push_back(make_tuple(w, u, v));
            }
        }
    }

    // Define comparator for sorting edges by weight
    struct CompareEdges {
        bool operator() (const tuple<double, unsigned int, unsigned int>& a, 
                        const tuple<double, unsigned int, unsigned int>& b) {
            return get<0>(a) < get<0>(b); // Compare based on weight
        }
    };

    // Sort edges by weight
    sort(edges.begin(), edges.end(), CompareEdges());

    UnionFind uf(num_nodes());

    // process edges in ascending order of weight
    for (size_t i = 0; i < edges.size(); i++) {
        double w = get<0>(edges[i]);
        unsigned int x = get<1>(edges[i]);
        unsigned int y = get<2>(edges[i]);
        uf.unionSet(x, y);

        if (uf.findSet(startID) == uf.findSet(endID)) {
            return w; // smallest threshold found
        }
    }

    return -1.0; // not connected after considering all edges
}
