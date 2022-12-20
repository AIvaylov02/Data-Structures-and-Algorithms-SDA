#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class StaticGraph
{
public:
    StaticGraph(int vertCount);
    StaticGraph(const StaticGraph& other) = delete;
    StaticGraph& operator=(const StaticGraph& other) = delete;
    ~StaticGraph();

    void AddEdge(int from, int to);
    void generateAllPaths();
    bool PathExists(int from, int to);
private:
    bool* visited = nullptr;
    int verticesCount;
    std::unordered_map<int, std::vector<int>> adjecent; // starting Adjacent nodes
    std::unordered_map<int, std::unordered_set<int>*> searchDict; // dict, which holds info for the different components(set). Each element of the set is a key to this set. That way we have const search time
    std::unordered_map<int, std::unordered_set<int>*> holdsMemoryVals; // used for cleaning up yhe memory from the dynamic sets (the uploaded solution had memory leakage)

    void free();
    void UtilDFS(int currNode, std::vector<int>& currNums, std::unordered_set<int>& pool);
};


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int vertices, edges;
    std::cin >> vertices >> edges;
    StaticGraph graph(vertices + 1); // Make the graph larger with digit, since we count from 1 to n, and not from 0 to n-1
    int from, to;

    // Fill the Graph
    for (int i = 0; i < edges; i++)
    {
        std::cin >> from >> to;
        graph.AddEdge(from, to); // since its undirected it should be bidirectional addition
        graph.AddEdge(to, from);
    }
    graph.generateAllPaths();

    int quaries;
    std::cin >> quaries;
    for (int i = 0; i < quaries; i++)
    {
        std::cin >> from >> to;
        std::cout << graph.PathExists(from, to) << ' ';
    }
    return 0;
}


StaticGraph::StaticGraph(int vertCount)
{
    if (vertCount < 0)
        verticesCount = 1;
    else
        verticesCount = vertCount;
    visited = new bool[verticesCount] {0};
}

void StaticGraph::free()
{
    verticesCount = 0;
    for (auto i = holdsMemoryVals.begin(); i != searchDict.end(); i++)
        delete i->second;
    delete[] visited;
}

StaticGraph::~StaticGraph()
{
    free();
}

void StaticGraph::AddEdge(int from, int to)
{
    adjecent[from].push_back(to);
}

bool StaticGraph::PathExists(int from, int to)
{
    return searchDict[from]->find(to) != searchDict[from]->end();
}

void StaticGraph::generateAllPaths()
{
    // iterate the whole map
    for (auto i = adjecent.begin(); i != adjecent.end(); i++)
    {
        if (!visited[(*i).first]) // if we haven't visited this node yet, start traversing it until we have found its component and set keys for each member
        {
            std::vector<int> nums;
            std::unordered_set<int>* pool = new std::unordered_set<int>();
            UtilDFS(i->first, nums, *pool); // creates the set and adds the nums(members of it) to a vector, thus allowing the creation of a useful dict with only 1 copy of the set
            holdsMemoryVals[i->first] = pool;
            for (auto vecIter = nums.begin(); vecIter != nums.end(); vecIter++) // add the pool of numbers to each of its participants (set the keys to correspond to the pool)
                searchDict[*vecIter] = pool;
        }

    }
}

void StaticGraph::UtilDFS(int currNode, std::vector<int>& keys, std::unordered_set<int>& pool) // creates the set and adds the nums(members of it) to a vector, thus allowing the creation of a useful dict with only 1 copy of the set
{
    visited[currNode] = true;
    pool.insert(currNode);
    keys.push_back(currNode);

    for (auto i = adjecent[currNode].begin(); i != adjecent[currNode].end(); i++)
    {
        if (!visited[*i])
            UtilDFS(*i, keys, pool);
    }
}