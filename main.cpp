#include <iostream>
#include <exception>

#include <main.h>

const string ROOT_DIRECTORY = "/";
const string PACMAN_DB_DIRECTORY = "/var/lib/pacman/";

int main()
{
    AlpmManager alpmManager(ROOT_DIRECTORY.c_str(), PACMAN_DB_DIRECTORY.c_str());
    GraphManager graphManager(alpmManager);

    graph_t g = graphManager.buildGraph(50);
    graphManager.printGraph(g, "/tmp/graph.dot");

    cout << "Graph generated!" << endl;

    graphManager.depthFirstSearch(g);

    return 0;
}
