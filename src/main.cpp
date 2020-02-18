#include <iostream>
#include <map>
#include <string>

#include <package_dependency.h>
#include <alpm_manager.h>
#include <graph_manager.h>

const char* ROOT_DIRECTORY = "/";
const char* PACMAN_DB_DIRECTORY = "/var/lib/pacman/";

using namespace std;

auto main() -> int
{
    // Getting local packages informations into GraphManager
    AlpmManager alpmManager(ROOT_DIRECTORY, PACMAN_DB_DIRECTORY);
    GraphManager graphManager(alpmManager);

    // Building graph
    graph_t g = graphManager.buildGraph();
    graphManager.printGraph(g, "/tmp/graph.dot");

    cout << "Graph generated!" << endl;

    // Finding dependencies of the following packages
    const vector<string> packages = { "bluez-utils" };
    auto results = graphManager.DFSFromMultipleVertices(g, packages, false);

    // Printing results...
    for_each(results.begin(), results.end(), [](string const &vertex)
    {
        cout << vertex << " ";
    });

    cout << endl;

    return 0;
}
