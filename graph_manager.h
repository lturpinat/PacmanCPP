#ifndef GRAPHMANAGER_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/property_map/property_map.hpp>

#include <alpm_manager.h>

#define GRAPHMANAGER_H

struct Vertex { std::string name; };
struct Edge { bool required; };

using graph_t  = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, Vertex, Edge >;

using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
using vertices_size_type = boost::graph_traits<graph_t>::vertices_size_type ;
using vertex_iterator = boost::graph_traits<graph_t>::vertex_iterator;

using edge_t = boost::graph_traits<graph_t>::edge_descriptor;
using out_edge_iterator = boost::graph_traits<graph_t>::out_edge_iterator ;
using edge_iterator = boost::graph_traits<graph_t>::edge_iterator ;

class GraphManager
{
private:
    AlpmManager manager;

    vertex_iterator findVertex(graph_t const &graph, const string& value);
    vector<string> mapVerticesIDToPackagesName(graph_t const &graph, vector<vertex_t> const &vertices);
    void DFSUtil(graph_t const &graph, vertex_t &vertex, vector<vertex_t> &visited);

public:
    GraphManager(AlpmManager manager);

    /**
     * @brief Build a graph using the cached packages of the pacman db associated to the alpm manager. The file is formated in .dot (Graphiz)
     * @return graph representing the relations between the packages
     */
    graph_t buildGraph();
    void printGraph(graph_t const &g, const char* filename);
    vector<string> DFS(graph_t const &graph);
    vector<string> DFSFromVertex(graph_t const &graph, const string& packageName);
    set<string> DFSFromMultipleVertices(graph_t &graph, const vector<string>& packagesNames);
};

#endif // GRAPHMANAGER_H
