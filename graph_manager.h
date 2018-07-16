#ifndef GRAPHMANAGER_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <alpm_manager.h>

#define GRAPHMANAGER_H

using namespace boost;

struct Vertex { std::string name; };
struct Edge { bool required; };

using graph_t  = adjacency_list<listS, vecS, directedS, Vertex, Edge >;
using vertex_t = graph_traits<graph_t>::vertex_descriptor;
using vertex_iterator = graph_traits<graph_t>::vertex_iterator;
using edge_t = graph_traits<graph_t>::edge_descriptor;

class GraphManager
{
private:
    AlpmManager manager;
    vertex_iterator findVertex(const graph_t& graph, const string& value);

public:
    GraphManager(AlpmManager manager);

    /**
     * @brief Build a graph using the cached packages of the pacman db associated to the alpm manager. The file is formated in .dot (Graphiz)
     * @param depth number of "main" packages processed. Default (all packages) is depth 0.
     * @return graph representing the relations between the packages
     */
    graph_t buildGraph(int depth = 0);
    void printGraph(graph_t g, const char* filename);
};

#endif // GRAPHMANAGER_H
