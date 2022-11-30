#include <iostream>

class BinaryTree
{
public:
	BinaryTree();
	BinaryTree(const BinaryTree& other) = delete;
	BinaryTree& operator=(const BinaryTree& other) = delete;
	~BinaryTree();

	int getMasksCount() const;
	void Insert(int currMask);
private:
	struct Node
	{
		Node* leftChild = nullptr;
		Node* rightChild = nullptr;
	};

	Node* root;
	int masksCount;
	void free(Node* currRoot);
	void insertUtil(int currMask, Node* root);
};

BinaryTree::BinaryTree()
{
	root = new Node();
	masksCount = 1; // 0 is always counted by default since it is a submask of every mask
}

BinaryTree::~BinaryTree()
{
	free(root);
}

void BinaryTree::free(Node* currRoot)
{
	if (currRoot != nullptr)
	{
		// we have to do a postOrder traversal in order not to have memory leak
		free(currRoot->leftChild);
		free(currRoot->rightChild);
		delete currRoot;
	}
}
int BinaryTree::getMasksCount() const
{
	return masksCount;
}

void BinaryTree::Insert(int currMask)
{
	insertUtil(currMask, root);
}

void BinaryTree::insertUtil(int currMask, Node* root)
{
	if (currMask != 0) // Since 0 will be treated as an always viable option
	{
		/* Here is the idea - in order for a number to be a submask it has to match 2 criterias:
		 First) It must be by default less or equal than the givenMask
		 Second) It mustn't have a bit of 1, where the given mask doesn't
		 => so theoretically we can start removing the last bit in order to slowly decrement the given mask
		 I will save the mask from lowest to largest value -> so the given value will be at the most right given spot */

		if (currMask % 2 == 1)
		{
			if (root->leftChild == nullptr) // we haven't already created a valid number from this combination of bits
			{
				root->leftChild = new Node;
				masksCount++;
			}
			insertUtil(currMask >> 1, root->leftChild);
		}

		if (root->rightChild == nullptr) // Balance out the tree with the same value
			root->rightChild = new Node;

		insertUtil(currMask >> 1, root->rightChild);
	}
}


int main()
{
	std::cin.tie(NULL);
	std::ios_base::sync_with_stdio(false);

	BinaryTree maskTree;
	int masksGivenByUser;
	std::cin >> masksGivenByUser;

	int currZoroMask;
	for (int i = 0; i < masksGivenByUser; i++)
	{
		std::cin >> currZoroMask;
		maskTree.Insert(currZoroMask);
	}

	int zoroApprovedMasks = maskTree.getMasksCount();
	std::cout << zoroApprovedMasks;
	return 0;
}