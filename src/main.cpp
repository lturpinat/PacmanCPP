#include <iostream>
#include <map>
#include <string>

#include <package_dependency.h>
#include <alpm_manager.h>
#include <graph_manager.h>

const string ROOT_DIRECTORY = "/";
const string PACMAN_DB_DIRECTORY = "/var/lib/pacman/";

using namespace std;

int main()
{
    // Getting local packages informations into GraphManager
    AlpmManager alpmManager(ROOT_DIRECTORY.c_str(), PACMAN_DB_DIRECTORY.c_str());
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
