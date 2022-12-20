#include <iostream>
#include <deque>
#include <vector>
#include <queue>
#include <unordered_map>


class StaticGraph
{
public:
	void FillDict(std::deque<std::string>& inputWords, int wordsCount);
	void TopologicalSort();
private:
	std::unordered_map<char, bool> visited;
	std::unordered_map<char, std::vector<char>> adjacent;
	
	void CompareWords(const std::string& firstWord, const std::string& secondWord);
	void AddEdge(char from, char to);
	void TopologicalSortUtil(char currNode, std::queue<char>& letters);
};



int main()
{
	using namespace std;
	int wordsCount;
	cin >> wordsCount;
	deque<string> dictionary;
	StaticGraph graph;

	for (int i = 0; i < wordsCount; i++)
	{
		string word;
		cin >> word;
		dictionary.push_back(word);
	}
	graph.FillDict(dictionary, wordsCount);
	graph.TopologicalSort();

	return 0;
}



void StaticGraph::AddEdge(char from, char to)
{
	adjacent[from].push_back(to);
}

void StaticGraph::CompareWords(const std::string& firstWord, const std::string& secondWord)
{
	int firstLen = firstWord.size();
	int secondLen = secondWord.size();
	int smallerLen = std::min(firstLen, secondLen);
	for (int currChar = 0; currChar < smallerLen; currChar++)
	{
		char firstL = firstWord[currChar];
		char secondL = secondWord[currChar];
		// there is misplacement of the letters
		if (firstL != secondL)
		{
			if (visited.find(firstL) == visited.end()) // if the character hasn't been already met, place it in the total letters dict
				visited[firstL] = false;
			if (visited.find(secondL) == visited.end()) // if the character hasn't been already met, place it in the total letters dict
				visited[secondL] = false;
			// create a directed edge from firstLetter to second
			AddEdge(secondL, firstL);
			break;
		}
	}
}


//  By travesing each word, create the whole dictionary of arrows from one character to another. This is a prerequisite for topological sort
void StaticGraph::FillDict(std::deque<std::string>& inputWords, int wordsCount)
{
	if (wordsCount == 1)
	{
		// tova tuka me sumnqva za validnost
		std::string word = inputWords[wordsCount-1];
		int wordSize = word.size();
		for (int i = 1; i < wordSize; i++)
		{
			if (!visited[word[i-1]])
			{
				visited[word[i-1]] = false;
				if (word[i - 1] != word[i])
					AddEdge(word[i - 1], word[i]);
			}
		}
		return;
	}
	for (int i = 1; i < wordsCount; i++)
	{
		std::string firstWord = inputWords[i - 1];
		std::string secondWord = inputWords[i];
		CompareWords(firstWord, secondWord);
	}
}

void StaticGraph::TopologicalSort()
{
	std::queue<char> letters;

	char currLetter;
	for (auto i = visited.begin(); i != visited.end(); i++)
	{
		currLetter = i->first;
		if (visited[currLetter] == false)
			TopologicalSortUtil(currLetter, letters);

		while (!letters.empty())
		{
			std::cout << letters.front() << ' ';
			letters.pop();
		}
	}
}

void StaticGraph::TopologicalSortUtil(char currNode, std::queue<char>& letters)
{
	// Mark the node as visited, traverse its neighbours first and add it to the stack at the end
	visited[currNode] = true;
	for (auto i = adjacent[currNode].begin(); i != adjacent[currNode].end(); i++)
	{
		char currLetter = *i;
		if (!visited[currLetter])
			TopologicalSortUtil(currLetter, letters);
	}

	letters.push(currNode);
}