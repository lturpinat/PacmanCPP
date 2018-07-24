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

    // Finding dependencies of <package>
    const string package = "avahi";
    auto results = graphManager.DFSFromVertex(g, package);

    cout << "Dependencies of \"" << package << "\" : ";

    // Printing results...
    for_each(results.begin(), results.end(), [](vertex_t const &vertex)
    {
        cout << vertex << " ";
    });

    cout << endl;

    return 0;
}
