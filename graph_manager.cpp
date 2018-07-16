#include "graph_manager.h"

GraphManager::GraphManager(AlpmManager manager) : manager(manager) {}

graph_t GraphManager::buildGraph(int depth)
{
    if(depth < 0) throw invalid_argument("Depth should be a positive number!");

    auto packages = manager.getPackages();

    graph_t g;

    int i = depth;

    // Adding vertexes
    for (const auto& kv : packages)
    {
        if(i == 0) break;

        vertex_t package = boost::add_vertex(Vertex{kv.first}, g);

        // Adding dependencies
        for(const auto& dependency : kv.second)
        {
            // Trying to find existing vertex for this dependency
            vertex_iterator vertex_dependency = findVertex(g, dependency->getPackageName());

            // Check if vertex_dependency isn't the end of the vertex vector
            if(vertex_dependency == vertices(g).second)
            {
                vertex_t new_vertex_dependency = boost::add_vertex(Vertex{dependency->getPackageName()}, g);
                boost::add_edge(package, new_vertex_dependency, Edge{ dependency->isRequired() }, g);
            }
            else
            {
                boost::add_edge(package, *vertex_dependency, Edge{ dependency->isRequired() }, g);
            }
        }

        i--;
    }

    return g;
}

void GraphManager::depthFirstSearch(graph_t const &graph)
{
    DFSCustomVisitor customVisitor;

    depth_first_search(graph, visitor(customVisitor));
}

void GraphManager::printGraph(graph_t const &g, const char* filename)
{
    ofstream file{};

    try {
        file.open (filename);

        boost::write_graphviz(file, g, [&] (ostream& out, vertex_t v) {
            out << "[label=\"" << g[v].name << "\"]";
        },
        [&] (ostream& out, edge_t e) {
            out << "[label=\"" << (g[e].required ? "required\"" : "optional\", color=\"blue\"") << "]";
        });

        file << flush;
    } catch (ofstream::failure e) {
        cerr << "Couldn't write within /tmp/graph.dot" << endl;
    }
}

vertex_iterator GraphManager::findVertex(const graph_t& graph, const string& value) {
    vertex_iterator vi, vi_end;

    for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
    {
        if(graph[*vi].name == value) return vi;
    }

    return vi_end;
}
