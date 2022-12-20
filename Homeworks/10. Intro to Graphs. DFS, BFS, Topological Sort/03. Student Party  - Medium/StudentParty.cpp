#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
const int DEFAULT_INVALID_VAL = -1;

// Note : If you create the whole inverse graph, you will get a slow solution. You don't need every piece of edge of the graph, only the interesting (most direct) ones

class StaticGraph
{
public:
	void AddEdge(int from, int to);
	void CalculateFriends(int friends, int startPerson);
	void PrintLens(int verticesCount, int startNumber) const;
private:
	struct Student
	{
		int pathlenToKoci = DEFAULT_INVALID_VAL;
	};

	std::vector<Student> students;
	// create a key to the correct adj indexes of the above array, that way we will make changes to students (This will be used to create inverse graph) 
	std::unordered_map<int, std::unordered_set<int>> adjacent; 
};


int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int queries;
	std::cin >> queries;
	
	for (int i = 0; i < queries; i++)
	{
		// obraboti 1 zaqvka
		StaticGraph graph;
		int vertices, edges;
		std::cin >> vertices >> edges;

		int from, to;
		for (int edge = 0; edge < edges; edge++)
		{
			std::cin >> from >> to;
			graph.AddEdge(from, to);
			graph.AddEdge(to, from);
		}
		// We have created until know the original graph
		int Koci;
		std::cin >> Koci;
		graph.CalculateFriends(vertices, Koci); // Creates only the needed(shortest path) inversed connections and adds number(pathlen) to each student 
		graph.PrintLens(vertices, Koci);
	}
	return 0;
}


void StaticGraph::AddEdge(int from, int to)
{
	adjacent[from].insert(to);
}


void StaticGraph::CalculateFriends(int vertices, int startPerson)
{
	// create the students array
	students = std::vector<Student>(vertices);

	std::unordered_map<int, bool> visited; // have a bool map which tells us which people have not been already dealt with, we just need only 1 connection to have shortest paths
	std::queue<int> friends;

	visited[startPerson - 1] = true;
	students[startPerson - 1].pathlenToKoci = 0; // mark Koci as visited and set its pathlen to zero since its the start of the array
	friends.push(startPerson);

	int personNum;
	while (!friends.empty())
	{
		personNum = friends.front();
		friends.pop();
		for (int friendNumber = 1; friendNumber <= vertices; friendNumber++)
		{
			if (visited.size() == vertices) // we have set aside all needed connection, stop iterating
				break;
			if (friendNumber == personNum)
				continue;
			if (adjacent[personNum].find(friendNumber) == adjacent[personNum].end()) // the connection is not present in the original graph
			{
				if (!visited[friendNumber - 1])
				{
					visited[friendNumber - 1] = true;
					students[friendNumber - 1].pathlenToKoci = students[personNum - 1].pathlenToKoci + 1; // set its pathlen and add it to the queue. That way we know for sure the optimal lenght of each element
					friends.push(friendNumber);
				}
			}

		}
	}
}

void StaticGraph::PrintLens(int verticesCount, int startNumber) const
{
	for (int i = 0; i < verticesCount; i++)
	{
		if (i != startNumber - 1)
			std::cout << students[i].pathlenToKoci << ' ';
	}
	std::cout << '\n';
}