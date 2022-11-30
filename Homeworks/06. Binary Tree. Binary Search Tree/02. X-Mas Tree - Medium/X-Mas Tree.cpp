#include <iostream>

//Alternative solution: We need a recursive function to tell if a tree is binary search tree
// a.k.a
// root > left && root < right
// dfs algo will be good enough - it is a binary tree(a.k.a max 3 neighbours, no conditions for it to be balanced)
// recursive solution will suffice if we could stop the insertion before the tree is finished

// We receive only a binary tree, which we will check if it is BTS
// The addition will be recursive and not relative

// Array of struct Nodes which we will compare
const int DEFAULT_EMPTY_VAL = -1;
struct Node
{
	int value = DEFAULT_EMPTY_VAL;
	Node* leftChild = nullptr;
	Node* rightChild = nullptr;
};

void FillArray(Node* baubles, int baublesCount); // the filling process is incremental, the search must not be by array consecutive index search
void TraverseTree(Node* current, int& currIndex, int* sortedValsArr);
bool IsBST(int* sortedValsArr, int baublesCount); // we will traverse the tree recursively and see if it follows order(for it to be BTS). We will use property of BTS inorder
// traversal, because then it must follow consecutive elements

int main()
{
	int trees;
	std::cin >> trees;
	int baublesCount; // christmas balls, nodes
	for (int i = 0; i < trees; i++)
	{
		std::cin >> baublesCount;
		Node* baubles = new Node[baublesCount];
		FillArray(baubles, baublesCount);
		int* sortedValsArr = new int[baublesCount];
		int currPutIndex = 0;
		TraverseTree(&baubles[0], currPutIndex, sortedValsArr);
		std::cout << IsBST(sortedValsArr, baublesCount) << '\n';
		delete[] sortedValsArr;
		delete[] baubles;
	}
	return 0;
}


void FillArray(Node* baubles, int baublesCount)
{
	int value, leftChild, rightChild;
	for (int i = 0; i < baublesCount; i++)
	{
		std::cin >> value >> leftChild >> rightChild;
		baubles[i].value = value;
		if (leftChild != -1) // the childs are nullptr by default
			baubles[i].leftChild = &baubles[leftChild];
		if (rightChild != -1)
			baubles[i].rightChild = &baubles[rightChild];
	}
}

// We will perform an inorder addition, thus meaning it should contain consecutive numbers. If they are not ordered somewhere, the bts function afterwards should catch it
void TraverseTree(Node* current, int& currIndex, int* sortedValsArr)
{
	if (current != nullptr)
	{
		TraverseTree(current->rightChild, currIndex, sortedValsArr);
		sortedValsArr[currIndex++] = current->value; // adds the value to the array and increments the index, sort of like print
		TraverseTree(current->leftChild, currIndex, sortedValsArr);
	}
}

bool IsBST(int* sortedValsArr, int baublesCount)
{
	// we should have a decreasing array of values
	for (int i = 1; i < baublesCount; i++)
	{
		if (sortedValsArr[i] >= sortedValsArr[i - 1]) 
			return false;
	}
	return true;
}
