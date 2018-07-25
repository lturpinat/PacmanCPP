#include <main.h>

const string ROOT_DIRECTORY = "/";
const string PACMAN_DB_DIRECTORY = "/var/lib/pacman/";

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
    const vector<string> packages = { "bind", "bind-tools", "avahi" };
    auto results = graphManager.DFSFromMultipleVertices(g, packages);

    // Printing results...
    for_each(results.begin(), results.end(), [](string const &vertex)
    {
        cout << vertex << " ";
    });

    cout << endl;

    return 0;
}
