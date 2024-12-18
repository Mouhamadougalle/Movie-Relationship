#include "LabeledGraph.h"
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

LabeledGraph::LabeledGraph(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('/');
        if (pos == std::string::npos) continue;

        std::string movie = line.substr(0, pos);
        add_vertex(movie);

        std::string actor_str = line.substr(pos + 1);
        std::stringstream ss(actor_str);
        std::string actor;
        while (std::getline(ss, actor, '/')) {
            // Convert "Last, First" to "First Last"
            size_t comma_pos = actor.find(',');
            if (comma_pos != std::string::npos) {
                std::string first = actor.substr(comma_pos + 2); // Skip ", "
                std::string last = actor.substr(0, comma_pos);
                actor = first + " " + last;
            }
            add_edge(movie, actor);
        }
    }
    file.close();
}


void LabeledGraph::add_vertex(string vlabel) {
   labels.push_back(vlabel);
   indexedLabels.insert(pair<string, int>(vlabel, V()));
   set<int> s;
   vertices.push_back(s);
}

void LabeledGraph::add_edge(string source, string target) {
    if (!contains(source)) add_vertex(source);
    if (!contains(target)) add_vertex(target);

    vertices[indexedLabels[source]].insert(indexedLabels[target]);
    vertices[indexedLabels[target]].insert(indexedLabels[source]);

    ++nedges;
}

bool LabeledGraph::contains(string vlabel) const {
    return indexedLabels.find(vlabel) != indexedLabels.end();
}

set<int> LabeledGraph::neighbors(int v) const {
    return vertices[v];
}

string LabeledGraph::label(int v) const {
    return labels[v];
}

int LabeledGraph::index(string vlabel) {
    return indexedLabels[vlabel];
}

/* 
BFS function
A queue helps explore vertices level by level, starting from the source vertex v.
A parent_map records which vertex led to each visited vertex.
BFS continues until all reachable vertices from the source are explored, marking each as visited to prevent revisiting.

This approach efficiently traverses the graph, making it useful for finding shortest paths and checking graph connectivity.
*/
map<int, int> LabeledGraph::BFS(int v) {
    map<int, int> parent_map;
    queue<int> to_visit;
    set<int> visited;

    to_visit.push(v);
    visited.insert(v);
    parent_map[v] = -1; // -1 signifies the root node

    while (!to_visit.empty()) {
        int current = to_visit.front();
        to_visit.pop();

        for (int neighbor : neighbors(current)) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                to_visit.push(neighbor);
                parent_map[neighbor] = current;
            }
        }
    }

    return parent_map;
}

vector<int> LabeledGraph::pathTo(map<int, int>& parent_map, int target) {
    vector<int> path;
    int current = target;

    while (current != -1) { // root node marked by -1
        path.push_back(current);
        current = parent_map[current];
    }

    reverse(path.begin(), path.end());
    return path;
}

ostream& operator<<(ostream& out, const LabeledGraph& g) {
    out << "==================================================================================\n";
    out << "Graph Summary: " << g.V() << " Vertices, " << g.E() << " Edges\n";
    out << "==================================================================================\n";
    for (int x = 0; x < g.V(); x++) {
        out << g.label(x) << "\n    ";
        auto neighbors = g.neighbors(x);
        for (auto it = neighbors.begin(); it != neighbors.end(); it++) {
            out << g.label(*it) << "\n    ";
        }
        out << "\n";
    }
    return out;
}

void LabeledGraph::display_related_vertices(string vlabel) {
    if (!contains(vlabel)) {
        cout << "Vertex with label '" << vlabel << "' not found in the graph." << endl;
        return;
    }

    int v = index(vlabel);
    cout << "These are directly related to " << vlabel << ":\n";
    for (int neighbor : neighbors(v)) {
        cout << "  " << label(neighbor) << endl;
    }
}