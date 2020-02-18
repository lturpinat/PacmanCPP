#include "graph_manager.h"

GraphManager::GraphManager(const AlpmManager &manager) : manager(manager) {}

graph_t GraphManager::buildGraph()
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

void GraphManager::DFSUtil(graph_t const &graph, vertex_t &vertex, vector<vertex_t> &visited, bool onlyRequiredDependencies)
{
    // Mark the current node as visited and print it
    visited.push_back(vertex);

    //Iterating recursively through the nearby vertices
    for ( std::pair<out_edge_iterator, out_edge_iterator> ve = boost::out_edges( vertex, graph );
          ve.first != ve.second; ++ve.first )
    {
        vertex_t v2 = boost::target( *ve.first, graph );

        //If the vertice has already been visited, skip it
        if(find(visited.begin(), visited.end(), v2) != visited.end())
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

vector<string> GraphManager::DFS(graph_t const &graph, bool onlyRequiredDependencies)
{
    unsigned long numberVertices = boost::num_vertices(graph);

    // Marking all vertices as not visited
    vector<vertex_t> visited(numberVertices);

    // Call the recursive helper function to print DFS traversal
    // starting from all vertices one by one
    for ( std::pair<vertex_iterator, vertex_iterator> vp = boost::vertices( graph );
          vp.first != vp.second; ++vp.first )
    {
        if(find(visited.begin(), visited.end(), *vp.first) != visited.end())
            continue;

        vertex_t v = *vp.first;
        DFSUtil(graph, v, visited, onlyRequiredDependencies);
    }

    // Clean the vector from [0,0,0,0,1,4,54] to [1,4,54] and remove from the list the very package vertex
    // id we are looking for.
    auto cleanedVisitedArray = remove_if(visited.begin(), visited.end(), [](vertex_t &vertex)
    { return vertex == static_cast< unsigned long >(0); });

    visited.erase(cleanedVisitedArray, visited.end());

    return mapVerticesIDToPackagesName(graph, visited);
}

/**
 * @brief GraphManager::DFSFromVertex
 * @param graph
 * @param packageName
 * @return a list of dependencies ID or an empty list if there are no related dependency found
 */
vector<string> GraphManager::DFSFromVertex(graph_t const &graph, const string& packageName, bool onlyRequiredDependencies)
{
    unsigned long numberVertices = boost::num_vertices(graph);

    // Initialize the vector at the number of vertices
    // TOFIX: using vectors "un-sized" is problematic as :
    // "If the new size() is greater than capacity() then all iterators
    // and references (including the past-the-end iterator) are invalidated.
    // Otherwise only the past-the-end iterator is invalidated."
    // (recursive calls in DFSUtil()
    vector<vertex_t> visited(numberVertices);

    auto vertexByName = findVertex(graph, packageName);
    if(vertexByName == vertices(graph).second)
        return vector<string>();

    vertex_t s = *vertexByName;

    DFSUtil(graph, s, visited, onlyRequiredDependencies);

    // Clean the vector from [0,0,0,0,1,4,54] to [1,4,54] and remove from the list the very package vertex
    // id we are looking for.
    auto cleanedVisitedArray = remove_if(visited.begin(), visited.end(), [&s](vertex_t &vertex)
    { return vertex == static_cast< unsigned long >(0) || vertex == s; });

    visited.erase(cleanedVisitedArray, visited.end());

    return mapVerticesIDToPackagesName(graph, visited);
}

set<string> GraphManager::DFSFromMultipleVertices(graph_t &graph, const vector<string>& packagesNames, bool onlyRequiredDependencies)
{
    set<string> requiredVertices;

    for(auto pkg : packagesNames)
    {
        auto dependencies = DFSFromVertex(graph, pkg, onlyRequiredDependencies);
        for_each(dependencies.begin(), dependencies.end(), [&requiredVertices](const string& val){
            requiredVertices.insert(val);
        });
    }

    return requiredVertices;
}

vector<string> GraphManager::mapVerticesIDToPackagesName(graph_t const &graph, vector<vertex_t> const &vertices)
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

        boost::write_graphviz(file, g, [&] (ostream& out, vertex_t v)
        {
            out << "[label=\"" << g[v].name << "\"]";
        },
        [&] (ostream& out, edge_t e)
        {
            out << "[label=\"" << (g[e].required ? "required\"" : "optional\", color=\"blue\"") << "]";
        });
    } catch (ofstream::failure & e)
    {
        cerr << "Couldn't write within /tmp/graph.dot" << endl;
    }
}

vertex_iterator GraphManager::findVertex(graph_t const &graph, const string& value)
{
    vertex_iterator vi, vi_end;

    for(boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
    {
        if(graph[*vi].name == value) return vi;
    }

    return vi_end;
}
