#include "LabeledGraph.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    LabeledGraph g("movies.txt");
    string source_label, target_label;

    cout << "Enter source vertex: ";
    getline(cin, source_label);

    if (!g.contains(source_label)) {
        cout << "Source vertex not found in the graph." << endl;
        return 1;
    }

    int source_index = g.index(source_label);
    auto parent_map = g.BFS(source_index);

    cout << "All (shortest) paths from " << source_label << " to other vertices have been created." << endl;

    while (true) {
        cout << "Enter a target vertex and I will display the shortest path from " << source_label
             << " to that vertex (-1 to exit): ";
        getline(cin, target_label);

        if (target_label == "-1") {
            cout << "Goodbye." << endl;
            break;
        }

        if (!g.contains(target_label)) {
            cout << "Target vertex not found in the graph." << endl;
            continue;
        }

        int target_index = g.index(target_label);
        if (parent_map.find(target_index) == parent_map.end()) {
            cout << "No path exists from " << source_label << " to " << target_label << "." << endl;
            continue;
        }

        auto path = g.pathTo(parent_map, target_index);
        cout << "Shortest Path from " << source_label << " to " << target_label << ":\n";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << g.label(path[i]);
            if (i < path.size() - 1) cout << " --> ";
        }
        cout << endl;
    }

    return 0;
}