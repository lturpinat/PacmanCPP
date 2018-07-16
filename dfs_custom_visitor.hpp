#ifndef DFS_CUSTOM_VISITOR_H

#include <boost/graph/depth_first_search.hpp>
#include <iostream>

#define DFS_CUSTOM_VISITOR_H

using namespace boost;

class DFSCustomVisitor : public boost::default_dfs_visitor
{
public:
    template< typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph& g) const
    {
        std::cout << "At " << u << std::endl;
    }

    template < typename Edge, typename Graph >
    void examine_edge(Edge e, const Graph &g) const
    {
        std::cout << "Examining edges "  << e << std::endl;
    }
};

#endif // DFS_CUSTOM_VISITOR_H
