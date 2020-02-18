#include "graph_manager.h"

using std::set;

GraphManager::GraphManager(const AlpmManager &manager) : manager(manager) {}

auto GraphManager::buildGraph() -> graph_t
{
    auto packages = manager.getPackages();

    graph_t g;

    int i = 50;

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

void GraphManager::DFSUtil(graph_t const &graph,const vertex_t &vertex, set<vertex_t> &visited, bool onlyRequiredDependencies)
{
    // Mark the current node as visited and print it
    visited.insert(vertex);

    //Iterating recursively through the nearby vertices
    for ( std::pair<out_edge_iterator, out_edge_iterator> ve = boost::out_edges( vertex, graph );
          ve.first != ve.second; ++ve.first )
    {
        vertex_t v2 = boost::target( *ve.first, graph );

        //If the vertice has already been visited, skip it
        if ( visited.find(v2) != end(visited) )
            continue;

        if(onlyRequiredDependencies)
        {
            auto edge = boost::edge(vertex, v2, graph);

            // If dependency is functional
            if(edge.second && graph[edge.first].required)
            {
                DFSUtil(graph, v2, visited, onlyRequiredDependencies);
            }
        }
        else
        {
            DFSUtil(graph, v2, visited, onlyRequiredDependencies);
        }
    }
}

auto GraphManager::DFS(graph_t const &graph, bool onlyRequiredDependencies) -> vector<string>
{
    // Marking all vertices as not visited
    set<vertex_t> visited;

    // Call the recursive helper function to print DFS traversal
    // starting from all vertices one by one
    for ( std::pair<vertex_iterator, vertex_iterator> vp = boost::vertices( graph );
          vp.first != vp.second; ++vp.first )
    {
        if ( visited.find(*vp.first) == end(visited) )
            DFSUtil(graph, *vp.first, visited, onlyRequiredDependencies);
    }

    visited.erase(0);

    return mapVerticesIDToPackagesName(graph, visited);
}

/**
 * @brief GraphManager::DFSFromVertex
 * @param graph
 * @param packageName
 * @return a list of dependencies ID or an empty list if there are no related dependency found
 */
auto GraphManager::DFSFromVertex(graph_t const &graph, const string& packageName, bool onlyRequiredDependencies) -> vector<string>
{
    // Initialize the vector at the number of vertices
    // TOFIX: using vectors "un-sized" is problematic as :
    // "If the new size() is greater than capacity() then all iterators
    // and references (including the past-the-end iterator) are invalidated.
    // Otherwise only the past-the-end iterator is invalidated."
    // (recursive calls in DFSUtil()
    set<vertex_t> visited;

    auto vertexByName = findVertex(graph, packageName);
    if(vertexByName == vertices(graph).second)
        return vector<string>();

    vertex_t s = *vertexByName;

    DFSUtil(graph, s, visited, onlyRequiredDependencies);

    visited.erase(0); // 2*log(n) < n if n big
    visited.erase(s);

    return mapVerticesIDToPackagesName(graph, visited);
}

auto GraphManager::DFSFromMultipleVertices(graph_t &graph, const vector<string>& packagesNames, bool onlyRequiredDependencies) -> set<string>
{
    set<string> requiredVertices;

    for(const auto& pkg : packagesNames)
    {
        auto dependencies = DFSFromVertex(graph, pkg, onlyRequiredDependencies);
        for_each(dependencies.begin(), dependencies.end(), [&requiredVertices](const string& val){
            requiredVertices.insert(val);
        });
    }

    return requiredVertices;
}

auto GraphManager::mapVerticesIDToPackagesName(graph_t const &graph, set<vertex_t> const &vertices) -> vector<string>
{
    vector<string> dependenciesAsString;
    std::transform(vertices.begin(), vertices.end(), back_inserter(dependenciesAsString), [ &graph ](const vertex_t& val)
    {
        return graph[val].name;
    });

    return dependenciesAsString;
}

void GraphManager::printGraph(graph_t const &g, const char* filename)
{

    try
    {
        ofstream file{filename};

        boost::write_graphviz(file, g,
            [&] (ostream& out, vertex_t v)
            {
                out << "[label=\"" << g[v].name << "\"]";
            },

            [&] (ostream& out, edge_t e)
            {
                out << "[label=\"" << (g[e].required ? "required\"" : R"(optional", color="blue")") << "]";
            });
    } catch (ofstream::failure & e)
    {
        cerr << "Couldn't write within /tmp/graph.dot" << endl;
    }
}

auto GraphManager::findVertex(graph_t const &graph, const string& value) -> vertex_iterator
{
    vertex_iterator vi, vi_end;

    for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
    {
        if(graph[*vi].name == value) return vi;
    }

    return vi_end;
}
