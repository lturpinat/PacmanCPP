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
using vertex_iterator = boost::graph_traits<graph_t>::vertex_iterator;
using edge_t = boost::graph_traits<graph_t>::edge_descriptor;

typedef boost::graph_traits<graph_t>::vertices_size_type vertices_size_type; // ie DigitalSurface::Size
typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;    // the iterator for visiting all vertices
typedef boost::graph_traits<graph_t>::out_edge_iterator out_edge_iterator;   // the iterator for visiting out edges of a vertex
typedef boost::graph_traits<graph_t>::edge_iterator edge_iterator;           // the iterator for visiting all edges

class GraphManager
{
private:
    AlpmManager manager;
    vertex_iterator findVertex(graph_t const &graph, const string& value);

public:
    GraphManager(AlpmManager manager);

    /**
     * @brief Build a graph using the cached packages of the pacman db associated to the alpm manager. The file is formated in .dot (Graphiz)
     * @return graph representing the relations between the packages
     */
    graph_t buildGraph();
    void printGraph(graph_t const &g, const char* filename);
    bool hasPath(graph_t const &graph, vertex_t src, vertex_t dest);
    void DFS(graph_t const &graph);
    void DFSUtil(graph_t const &graph, vertex_t &vertex, vector<vertex_t> &visited);
    vector<vertex_t> DFSFromVertex(graph_t const &graph, const string& packageName);
};

#endif // GRAPHMANAGER_H
